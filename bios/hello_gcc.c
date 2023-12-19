#include <efi.h>
#include <lib.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS status;
    InitializeLib(image, SystemTable);

    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");
    Print(L"Hello World: GCC GNU-EFI\n");

  return EFI_SUCCESS;
}
