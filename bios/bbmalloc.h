//-------------------------------------------------------------------------------------
void check_membounds() {
        Print(L"bbmalloc(): check alloc bounds \n");
}    
//-------------------------------------------------------------------------------------
void * bbmalloc(UINTN poolSize)
{
    Print(L"bbmalloc(): align and pad 16-bytes to alloc size\n");

    poolSize = (poolSize | 0xF) + 0x9 ;
    check_membounds();
    
    EFI_STATUS status;
    void * handle;
    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, poolSize, &handle);

    if(status == EFI_OUT_OF_RESOURCES)
    {
        Print(L"bbmalloc(): out of resources for pool\n");
        return 0;
    }
    else if(status == EFI_INVALID_PARAMETER)
    {
        Print(L"bbmalloc(): invalid pool type\n");
        return 0;
    }
    else
    {
        Print(L"bbmalloc(): memory pool successfully allocated %d\n", poolSize);
        return handle;
    }
}

void bbfree(void * pool)
{
    EFI_STATUS status;
    //Print(L"bbfree(): freeing memory pool\n");
    status = uefi_call_wrapper(BS->FreePool, 1, pool);

    if(status == EFI_INVALID_PARAMETER)
    {
        Print(L"bbfree(): invalid pool pointer\n");
    }
    else
    {
        Print(L"bbfree(): memory pool successfully freed\n");
    }
}

//-------------------------------------------------------------------------------------
