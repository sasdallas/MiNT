/**
 * @file boot/ldr/ldr.c
 * @brief PE loader
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <ldr/ldr.h>
#include <mm/mm.h>
#include <dbg.h>
#include <minilibc/string.h>

INT LdrImageLoad(UINT_PTR Base, MINTLDR_MEMORY_TYPE MemoryType, PVOID *ImageBase) {
    PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)Base;
    
    /* Validate MZ magic */
    if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        ERROR("DOS signature not matched: %4x\n", DosHeader->e_magic);
        return 1;
    }

    /* Use DOS header to get NT headers */
    PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)(Base + DosHeader->e_lfanew);

    /* Validate signature magic */
    if (NtHeaders->Signature != IMAGE_NT_SIGNATURE) {
        ERROR("NT signature not matched: %8x\n", NtHeaders->Signature);
        return 1;
    }

    DEBUG("Image base: %16x Entrypoint: %16x\n", NtHeaders->OptionalHeader.ImageBase, NtHeaders->OptionalHeader.AddressOfEntryPoint);

    /* TODO: Handle failure and relocation */
    PMINTLDR_MEMORY_REGION Region;
    UINT_PTR ExecutableBase = MmAllocatePagesAtAddress(NtHeaders->OptionalHeader.SizeOfImage / MM_PAGE_SIZE, NtHeaders->OptionalHeader.ImageBase, &Region);
    Region->MemoryType = RegionKernel;

    /* Copy the headers to the executable base */
    memcpy((PVOID)ExecutableBase, (PVOID)Base, NtHeaders->OptionalHeader.SizeOfHeaders);

    /* Reget the NT headers */
    DosHeader = (PIMAGE_DOS_HEADER)ExecutableBase;
    NtHeaders = (PIMAGE_NT_HEADERS)(ExecutableBase + DosHeader->e_lfanew);

    /* Start processing section entries */
    PIMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(NtHeaders);

    for (SIZE_T i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++) {
        DEBUG("Section \"%s\": virtual address 0x%16x phys addr 0x%16x\n", Section->Name, ExecutableBase + Section->VirtualAddress, Section->PointerToRawData);

        memcpy((PVOID)ExecutableBase + Section->VirtualAddress, (PVOID)Base + Section->PointerToRawData, Section->SizeOfRawData);
        Section++;
    }

    ((void (*)())((ULONG_PTR)ExecutableBase + NtHeaders->OptionalHeader.AddressOfEntryPoint))();
    DEBUG("Error..\n");
    return 1;
}