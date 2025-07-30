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
#include <ui.h>

PMINTLDR_LOADED_IMAGE LoadedImageList = NULL;

PIMAGE_SECTION_HEADER LdrFindImageSection(UINT_PTR ImageBase, PCHAR SectionName) {
    /* Get the NT headers */
    PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)ImageBase;
    PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)(ImageBase + DosHeader->e_lfanew);

    /* Start processing section entries */
    PIMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(NtHeaders);

    for (SIZE_T i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++) {
        if (!strcmp(Section->Name, SectionName)) {
            return Section;
        }

        Section++;
    }

    /* Section not found */
    return NULL;
} 

BOOL LdrFindDllByName(PCHAR DllName, PMINTLDR_LOADED_IMAGE *LoadedImage) {
    PMINTLDR_LOADED_IMAGE Image = LoadedImageList;
    while (Image) {
        if (!strcmp(Image->Name, DllName)) {
            if (LoadedImage) *LoadedImage = Image;
            return TRUE;
        }

        Image = Image->NextImage;
    }

    return FALSE;
}

BOOL LdrCheckIfDllLoaded(PCHAR DllName) {
    PMINTLDR_LOADED_IMAGE Image = LoadedImageList;
    while (Image) {
        if (!strcmp(Image->Name, DllName)) {
            return TRUE;
        }

        Image = Image->NextImage;
    }

    return FALSE;
}

INT LdrProcessExportTable(PMINTLDR_LOADED_IMAGE LoadedImage, UINT_PTR ExecutableBase) {
    PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)ExecutableBase;
    PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)(ExecutableBase + DosHeader->e_lfanew);

    /* Try to get the export table section */
    PIMAGE_DATA_DIRECTORY ExportDataDirectory = &(NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
    
    if (!ExportDataDirectory->Size) {
        return 0; /* No exports */
    }

    PIMAGE_EXPORT_DIRECTORY ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((UINT_PTR)ExecutableBase + (UINT_PTR)ExportDataDirectory->VirtualAddress);

    /* Start going through each export */
    PDWORD ExportNameTable = (PDWORD)(ExecutableBase + ExportDirectory->AddressOfNames);
    PSHORT NameOrdinalsTable = (PSHORT)(ExecutableBase + ExportDirectory->AddressOfNameOrdinals);
    PDWORD ExportAddressTable = (PDWORD)(ExecutableBase + ExportDirectory->AddressOfFunctions);

    for (INT i = 0; i < ExportDirectory->NumberOfNames; i++) {
        PCHAR Name = (PCHAR)(ExecutableBase + ExportNameTable[i]);

        /* Build the export entry */
    extern MINTLDR_HEAP MintInitialHeap;
        PMINTLDR_IMAGE_EXPORT Export = MmAllocateHeap(MintDefaultHeap, sizeof(MINTLDR_IMAGE_EXPORT));
        Export->Name = Name;

        /* Calculate the export address */
        WORD Ordinal = NameOrdinalsTable[i];
        Export->Address = (UINT_PTR)(ExecutableBase + ExportAddressTable[Ordinal]);
        
        DEBUG("%s @ %16x\n", Export->Name, Export->Address);

        Export->Next = LoadedImage->ImageExportList;
        LoadedImage->ImageExportList = Export;
    }

    return 0;
}

INT LdrLookupExportFromDLL(PMINTLDR_LOADED_IMAGE Dll, PIMAGE_IMPORT_BY_NAME Hint, PIMAGE_THUNK_DATA IATEntry) {
    /* Look through each export */
    PMINTLDR_IMAGE_EXPORT Export = Dll->ImageExportList;
    while (Export) {
        /* TODO: Hint + ordinal? PLEASE optimize this.. */
        if (!strcmp(Export->Name, Hint->Name)) {
            INFO("Found symbol %s @ %16x - updating ILT entry 0x%16x\n", Hint->Name, Export->Address, IATEntry);
            IATEntry->u1.Function = Export->Address;

            return 0;
        }

        Export = Export->Next;
    }

    return 1;
}

INT LdrProcessImportTable(PMINTLDR_LOADED_IMAGE LoadedImage, UINT_PTR ExecutableBase) {
    PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)ExecutableBase;
    PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)(ExecutableBase + DosHeader->e_lfanew);

    /* Try to get the .idata section */
    PIMAGE_DATA_DIRECTORY ImportDataDirectory = &(NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
    PIMAGE_IMPORT_DESCRIPTOR Descriptor = (PIMAGE_IMPORT_DESCRIPTOR)((UINT_PTR)ExecutableBase + (UINT_PTR)ImportDataDirectory->VirtualAddress);

    INT Error = 0;

    while (Descriptor->Name) {
        DEBUG("Image requires \'%s\'\n", ExecutableBase + Descriptor->Name);

        /* Look for the DLL */
        UINT_PTR DllBase = NULL;
        PMINTLDR_LOADED_IMAGE LoadedDLL = NULL;
        if (!LdrFindDllByName((PCHAR)(ExecutableBase + Descriptor->Name), &LoadedDLL)) {
            /* Locate this DLL */
            DllBase = LdrArchImageLookup((PCHAR)(ExecutableBase + Descriptor->Name));
            if (DllBase) {
                /* Temporary - TODO: We can get away with not loading these */
                INFO("Found DLL \"%s\" at %16x\n", ExecutableBase + Descriptor->Name, DllBase);

                /* The DLL has been loaded, we can now load it into memory */
                if (LdrImageLoadEx((PCHAR)(ExecutableBase + Descriptor->Name), DllBase, RegionDll, NULL, &LoadedDLL)) {
                    /* Temporary - TODO: We can get away with not loading these */
                    MintBugCheckWithMessage(DLL_CORRUPTED, "Failed to load required DLL file \"%s\"\n", ExecutableBase + Descriptor->Name);
                }
            }
        } else {
            DllBase = (UINT_PTR)LoadedDLL->LoadBase;
        }

        if (!DllBase) {
            /* Temporary - TODO: We can get away with not loading these */
            MintBugCheckWithMessage(FILE_NOT_FOUND, "Missing required DLL file \"%s\"\n", ExecutableBase + Descriptor->Name);
        }

    

        /* Now we can start looking through each required function */
        /* Get the ILT */
    #ifdef _WIN64
        unsigned long long bit = (1 << 63);
    #else
        unsigned long long bit = (1 << 31);
    #endif

        PIMAGE_THUNK_DATA ImportLookupTable = (PIMAGE_THUNK_DATA)(ExecutableBase + Descriptor->OriginalFirstThunk);
        PIMAGE_THUNK_DATA ImportAddressTable = (PIMAGE_THUNK_DATA)(ExecutableBase + Descriptor->FirstThunk);

        while (ImportLookupTable->u1.Function) {
            DEBUG("ILT: Function=%8x Ordinal=%8x AddressOfData=%16x\n", ImportLookupTable->u1.Function, ImportLookupTable->u1.Ordinal, ImportLookupTable->u1.AddressOfData);
            
            if (ImportLookupTable->u1.Ordinal & IMAGE_ORDINAL_FLAG) {
                /* Ordinal, damn. TODO */
                MintBugCheckWithMessage(DLL_CORRUPTED, "DLL \"%s\" has ordinal entry (unimplemented)\n", LoadedImage->Name);
            } else {
                /* Normal, yay */
                PIMAGE_IMPORT_BY_NAME HintName = (PIMAGE_IMPORT_BY_NAME)(ExecutableBase + (UINT_PTR)(ImportLookupTable->u1.AddressOfData));
                DEBUG("Require symbol hint=%4x %s\n", HintName->Hint, HintName->Name);

                if (LdrLookupExportFromDLL(LoadedDLL, HintName, ImportAddressTable)) {
                    ERROR("Failed to find symbol in \"%s\": %s\n", LoadedDLL->Name, HintName->Name);
                    Error = 1;
                }
            }

            ImportLookupTable++;
            ImportAddressTable++;
        }

        /* Next descriptor */
        Descriptor++;
    }

    return Error;
}

INT LdrImageLoadEx(PCHAR ImageName, UINT_PTR Base, MINTLDR_MEMORY_TYPE MemoryType, PVOID *ImageBase, PMINTLDR_LOADED_IMAGE *LoadedImageOut) {
    /* Temporary status message */
    UiPrint("Loading %s\n", ImageName);

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
    UINT_PTR ExecutableBase = MmAllocatePagesAtAddress(MM_PAGE_ALIGN_UP(NtHeaders->OptionalHeader.SizeOfImage) / MM_PAGE_SIZE, (UINT_PTR)NtHeaders->OptionalHeader.ImageBase, &Region);
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

    /* Create the entry for this image */
    PMINTLDR_LOADED_IMAGE LoadedImageObject = MmAllocateHeap(MintDefaultHeap, sizeof(MINTLDR_LOADED_IMAGE));
    LoadedImageObject->Name = ImageName;
    LoadedImageObject->ImageExportList = NULL;
    LoadedImageObject->LoadBase = (PVOID)ExecutableBase;
    LoadedImageObject->PrevImage = NULL;
    LoadedImageObject->Size = MM_PAGE_ALIGN_UP(NtHeaders->OptionalHeader.SizeOfImage);
    LoadedImageObject->NextImage = LoadedImageList;
    LoadedImageObject->Entrypoint = (PVOID)((UINT_PTR)ExecutableBase + NtHeaders->OptionalHeader.AddressOfEntryPoint);

    if (LoadedImageList) {
        LoadedImageList->PrevImage = LoadedImageObject;
    }    

    LoadedImageList = LoadedImageObject;

    /* Process and build the export table */
    LdrProcessExportTable(LoadedImageObject, ExecutableBase);

    /* Start processing the import table */
    LdrProcessImportTable(LoadedImageObject, ExecutableBase);

    DEBUG("Final image: Name=\"%s\" LoadBase=%16x Entry=%16x\n", LoadedImageObject->Name, LoadedImageObject->LoadBase, LoadedImageObject->Entrypoint);
    if (LoadedImageOut) *LoadedImageOut = LoadedImageObject;
    return 0;
}

INT LdrImageLoad(PCHAR ImageName, UINT_PTR Base, MINTLDR_MEMORY_TYPE MemoryType, PVOID *ImageBase) {
    return LdrImageLoadEx(ImageName, Base, MemoryType, ImageBase, NULL);
}

UINT_PTR LdrImageGetEntrypoint(UINT_PTR ImageBase, PVOID LoadBase) {
    PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)ImageBase;
    PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)(ImageBase + DosHeader->e_lfanew);

    return (UINT_PTR)LoadBase +  NtHeaders->OptionalHeader.AddressOfEntryPoint;
}  