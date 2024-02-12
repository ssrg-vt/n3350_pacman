#include <efi.h>
#include <lib.h>
#include <efilib.h>
#include <stdlib.h>
#define _EFI_MP_
#include "efi-mp.h"
#include "raw_msr.h"
#include "raw_perf.h"
#include "goldmont_core_perf_counters.h"
//#include "exceptions.h"
#include "malloc.h"
#include "bbmalloc.h"

#define mfence() asm volatile("mfence\n")
#define lfence() asm volatile("lfence\n")
#define lmfence() asm volatile("lfence\n mfence\n")
#define wbinvd() asm volatile("wbinvd\n")
#define barrier() asm volatile("xor %%rax, %%rax\n xor %%rcx, %%rcx\n cpuid\n":::"cc", "rax", "rbx", "rcx", "rdx")

#define time_access(ptr)({\
    register uint32_t delta;\
    asm volatile(\
      "mov %%rax, %%r10\n"\
      "mfence\n"\
      "rdtscp\n"\
      "mov %%rax, %%r11\n"\
      "mov (%%rbx), %%rcx\n"\
      "lfence\n"\
      "rdtscp\n"\
      "sub %%rax, %%r11\n"\
      "mov %%r10, %%rax\n"\
      "neg %%r11\n"\
      "mov %%r11, %%rcx"\
      : "=c" (delta)\
      : "b" (ptr)\
      : "rdx", "r11", "r10"\
    );\
    delta;})

#define EFI_SHELL_INTERFACE_GUID \
   (EFI_GUID) {0x47c7b223, 0xc42a, 0x11d2, {0x8e,0x57,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

#define SHELL_VARIABLE_GUID \
   (EFI_GUID) {0x158def5a, 0xf656, 0x419c, {0xb0,0x27,0x7a,0x31,0x92,0xc0,0x79,0xd2}}

typedef enum {
    ARG_NO_ATTRIB         = 0x0,
    ARG_IS_QUOTED         = 0x1,
    ARG_PARTIALLY_QUOTED  = 0x2,
    ARG_FIRST_HALF_QUOTED = 0x4,
    ARG_FIRST_CHAR_IS_ESC = 0x8
} EFI_SHELL_ARG_INFO_TYPES;

struct _EFI_SHELL_ARG_INFO {
    UINT32 Attributes;
} __attribute__((packed)) __attribute__((aligned (1)));
typedef struct _EFI_SHELL_ARG_INFO EFI_SHELL_ARG_INFO;

struct _EFI_SHELL_INTERFACE {
    EFI_HANDLE           ImageHandle;
    EFI_LOADED_IMAGE    *Info;
    CHAR16             **Argv;
    UINTN                Argc;
    CHAR16             **RedirArgv;
    UINTN                RedirArgc;
    EFI_FILE            *StdIn;
    EFI_FILE            *StdOut;
    EFI_FILE            *StdErr;
    EFI_SHELL_ARG_INFO  *ArgInfo;
    BOOLEAN              EchoOn;
} __attribute__((packed)) __attribute__((aligned (1)));
typedef struct _EFI_SHELL_INTERFACE EFI_SHELL_INTERFACE;

static EFI_STATUS
get_args(EFI_HANDLE image, UINTN *argc, CHAR16 ***argv)
{
    EFI_STATUS status;
    EFI_SHELL_INTERFACE *shell;
    EFI_GUID gEfiShellInterfaceGuid = EFI_SHELL_INTERFACE_GUID;

    status = uefi_call_wrapper(BS->OpenProtocol, 6,
                               image, &gEfiShellInterfaceGuid,
                               (VOID **)&shell, image, NULL,
                               EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(status))
        return status;

    *argc = shell->Argc;
    *argv = shell->Argv;

    status = uefi_call_wrapper(BS->CloseProtocol, 4, image,
                               &gEfiShellInterfaceGuid, image, NULL);
    return status;
}

//-------------------------------------------------------------------------------------
/*
 * steam.c - lmbench version of John McCalpin's STREAM benchmark
 *
 * usage: stream
 *
 * Copyright (c) 2000 Carl Staelin.
 * Copyright (c) 1994 Larry McVoy.  Distributed under the FSF GPL with
 * additional restriction that results may published only if
 * (1) the benchmark is unmodified, and
 * (2) the version in the sccsid below is included in the report.
 * Support for this development by Sun Microsystems is gratefully acknowledged.
 */
//-------------------------------------------------------------------------------------
char	*id = "$Id$\n";

//-- #include "bench.h"

struct _state {
	int	len;
	double*	a;
	double*	b;
	double*	c;
	double	scalar;
};

#define BODY(expr)							        \
{									                \
	register int i;							        \
	register int N = state->len;					\
	register double* a = state->a;					\
	register double* b = state->b;					\
	register double* c = state->c;					\
	register double scalar = state->scalar;			\
									                \
	state->a = state->b;				            \
	state->b = state->c;				            \
	state->c = a;						            \
									                \
	for (i = 0; i < N; ++i) {			            \
		expr;							            \
	}								                \
}


//-- void initialize(iter_t iterations, void* cookie);
//-- void cleanup(iter_t iterations, void* cookie);
//-- void initialize(uint32_t iterations, void* cookie);
void cleanup(uint32_t iterations, void* cookie);

//-- /* These are from STREAM version 1 */
//-- void copy(iter_t iterations, void* cookie);
//-- void scale(iter_t iterations, void* cookie);
//-- void add(iter_t iterations, void* cookie);
//-- void triad(iter_t iterations, void* cookie);
//-- 
//-- /* These are from STREAM version 2 */
//-- void fill(iter_t iterations, void* cookie);
//-- /* NOTE: copy is the same as in version 1 */
//-- void daxpy(iter_t iterations, void* cookie);
//-- void sum(iter_t iterations, void* cookie);

/* These are from STREAM version 1 */
void copy  (uint32_t iterations, void* cookie);
void scale (uint32_t iterations, void* cookie);
void add   (uint32_t iterations, void* cookie);
void triad (uint32_t iterations, void* cookie);

/* These are from STREAM version 2 */
void fill  (uint32_t iterations, void* cookie);
/* NOTE: copy is the same as in version 1 */
void daxpy (uint32_t iterations, void* cookie);
void sum   (uint32_t iterations, void* cookie);

//---------------------------------------------------------
// *
// * Assumptions:
// *
// * 1) Cache lines are a multiple of pointer-size words
// * 2) Cache lines are no larger than 1/4 a page size
// * 3) Pages are an even multiple of cache lines
// *
//---------------------------------------------------------

void initialize(uint32_t iterations, void* cookie)
//void initialize(uint32_t iterations)
{
	int i;
	struct _state* state = (struct _state*)cookie;
	
	// if (iterations) return;

	state->a = (double*)bbmalloc(sizeof(double) * state->len);
	state->b = (double*)bbmalloc(sizeof(double) * state->len);
	state->c = (double*)bbmalloc(sizeof(double) * state->len);

	if (state->a == NULL || state->b == NULL || state->c == NULL) {
	  //--	exit(1);
	}

	for (i = 0; i < state->len; ++i) {
		state->a[i] = 1.;
		state->b[i] = 2.;
		state->c[i] = 0.;
	}
}

void copy(uint32_t iterations, void *cookie)
{
	struct _state* state = (struct _state*)cookie;

	while (iterations-- > 0) {
		BODY(c[i] = a[i];)
	}
}

void scale(uint32_t iterations, void *cookie)
{
	struct _state* state = (struct _state*)cookie;

	while (iterations-- > 0) {
		BODY(b[i] = scalar * c[i];)
	}
}

void add(uint32_t iterations, void *cookie)
{
	struct _state* state = (struct _state*)cookie;

	while (iterations-- > 0) {
		BODY(c[i] = a[i] + b[i];)
	}
}

void triad(uint32_t iterations, void *cookie)
{
	struct _state* state = (struct _state*)cookie;

	while (iterations-- > 0) {
		BODY(a[i] = b[i] + scalar * c[i];)
	}
}

//-- *
//-- * STREAM version 2 benchmark kernels
//-- * NOTE: copy is the same as version 1's benchmark
//-- *
void fill(uint32_t iterations, void *cookie)
{
	struct _state* state = (struct _state*)cookie;

	while (iterations-- > 0) {
		BODY(a[i] = 0;)
	}
}

void daxpy(uint32_t iterations, void *cookie)
{
	struct _state* state = (struct _state*)cookie;

	while (iterations-- > 0) {
		BODY(a[i] = a[i] + scalar * b[i];)
	}
}

void sum(uint32_t iterations, void *cookie)
{
	register double	s;
	struct _state* state = (struct _state*)cookie;

	s = 0.0;
	while (iterations-- > 0) {
		BODY(s += a[i];)
	}
	//-- use_int((int)s);
}

void cleanup(uint32_t iterations, void* cookie)
{
	struct _state* state = (struct _state*)cookie;

	if (iterations) return;

	//-- free(state->a);
	//-- free(state->b);
	//-- free(state->c);
}

//-------------------------------------------------------------------------------------
EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *SystemTable)
{

    UINTN argc;
    CHAR16 **argv;

    EFI_STATUS status;
    EFI_TIME Time;

    InitializeLib(image, SystemTable);

    //-- status = get_args(image, &argc, &argv);
    //-- if (EFI_ERROR(status)) {
    //--     Print(L"ERROR: Parsing command line arguments: %d\n", status);
    //--     return status;
    //-- }

    //-- if (StrCmp(argv[1], L"patch") == 0) {
    //--      Print(L"[patch]\n");
    //--  }
    //-- 
    //--  if (StrCmp(argv[1], L"malloc") == 0) {
    //--      Print(L"[malloc]\n");
    //--  }

    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"LMBench Stream test Begins...  Lalit\n");

    //----------    
 	int	   i, j, l;
 	int	   version = 1;
 	int	   parallel = 1;
 	int	   warmup = 0;
 	int	   repetitions = 1000;
 	int	   c;
 	int	   datasize;
 	struct _state state;
 	char   *p;
 	char   *usage = "[-v <stream version 1|2>] [-M <len>[K|M]] [-P <parallelism>] [-W <warmup>] [-N <repetitions>]\n";
    float  bw=0.0;
    double ns=0;
    double seconds=0;
    double start_sec=0;
    double end_sec=0;
    double start_ns=0;
    double end_ns=0;

    double* test1;
    double* test2;
    double* test3;

    state.len = (1000000 * sizeof(double)) + 3;
 	state.scalar = 3.0;

    Print(L"Allocated Memory to Struct State.A [%d] \n", sizeof(double)*state.len );
    Print(L"Allocated Memory to Struct State.B [%d] \n", sizeof(double)*state.len );
    Print(L"Allocated Memory to Struct State.C [%d] \n", sizeof(double)*state.len );
	state.a = (double*)bbmalloc(sizeof(double) * state.len);
	state.b = (double*)bbmalloc(sizeof(double) * state.len);
	state.c = (double*)bbmalloc(sizeof(double) * state.len);

    //** -----
	for (i = 0; i < state.len; ++i) {
		state.a[i] = 0xDEADBEEF;
		state.b[i] = 0xDEADBEEF;
		state.c[i] = 0xDEADBEEF;
	}

    barrier();

	for (i = 0; i < state.len; ++i) {
		state.a[i] = 0x1;
		state.b[i] = 0x2;
		state.c[i] = 0xF;
	}

    Print(L"-------------------------------------\n");
    Print(L"STREAM fill latency    : %ld seconds \n", time_access(&state.a[0]));
    Print(L"STREAM fill bandwidth  : %ld B/s\n", 64000000/time_access(&state.a[0]));

	for (i = 0; i < state.len; ++i) {
		state.c[i] = state.b[i];
		state.b[i] = state.a[i];
		state.a[i] = state.c[i];
	}

    Print(L"STREAM copy latency    : %ld seconds \n", time_access(&state.a[0]));
    Print(L"STREAM copy bandwidth  : %ld B/s\n", 64000000/time_access(&state.a[0]));
    barrier();

	for (i = 0; i < state.len; ++i) {
		state.c[i] = state.a[i] + state.b[i];
		state.b[i] = state.a[i] + state.c[i];
		state.a[i] = state.b[i] + state.c[i];
	}

    Print(L"STREAM add latency     : %ld seconds \n", time_access(&state.a[0]));
    Print(L"STREAM add bandwidth   : %ld B/s\n", 64000000/time_access(&state.a[0]));
    barrier();

	for (i = 0; i < state.len; ++i) {
		state.a[i] = state.a[i] + state.scalar*state.b[i];
		state.b[i] = state.a[i] + state.scalar*state.c[i];
		state.a[i] = state.b[i] + state.scalar*state.c[i];
	}

    Print(L"STREAM triad latency   : %0ld seconds \n", time_access(&state.a[0]));
    Print(L"STREAM triad bandwidth : %ld B/s\n", 64000000/time_access(&state.a[0]));
    Print(L"-------------------------------------\n");

	bbfree(state.a);
	bbfree(state.b);
	bbfree(state.c);

    //----------    
    Print(L"LMBench StreamV2 malloc test Ends... Lalit\n");

  return EFI_SUCCESS;
}

//-------------------------------------------------------------------------------------

