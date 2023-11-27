#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
InitializeLib(ImageHandle, SystemTable);
  Print(L"JMD!\n");
  Print(L"HELLO WORLD! #1\n");
  Print(L"HELLO WORLD! #2\n");
  Print(L"HELLO WORLD! #3\n");
  Print(L"HELLO WORLD! #4\n");
  Print(L"HELLO WORLD! #5\n");
  
return EFI_SUCCESS;
}
