//-------------------------------------------------------------------------------------
void * malloc(UINTN poolSize)
{
    EFI_STATUS status;
    void * handle;
    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, poolSize, &handle);

    if(status == EFI_OUT_OF_RESOURCES)
    {
        Print(L"malloc(): out of resources for pool\n");
        return 0;
    }
    else if(status == EFI_INVALID_PARAMETER)
    {
        Print(L"malloc(): invalid pool type\n");
        return 0;
    }
    else
    {
        Print(L"malloc(): memory pool successfully allocated for %d\n", poolSize);
        return handle;
    }
}

void free(void * pool)
{
    EFI_STATUS status;
    status = uefi_call_wrapper(BS->FreePool, 1, pool);

    if(status == EFI_INVALID_PARAMETER)
    {
        Print(L"bbfree(): invalid pool pointer\n");
    }
    else
    {
        Print(L"free(): memory pool successfully freed\n");
    }
}
//-------------------------------------------------------------------------------------

