/**
 * @file boot/arch/x86_64/main.c
 * @brief MiNT loader main
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <winnt.h>
#include <ntdef.h>
#include <ui.h>
#include <dbg.h>
#include <multiboot.h>
#include <mm/mm.h>
#include <arch/serial.h>

/* Memory region list */
MINTLDR_MEMORY_REGION MemoryRegionList[81];

MINTLDR_MEMORY_TYPE MintLoaderMultibootToLoaderType(INT Type) {
    switch (Type) {
        case MULTIBOOT_MEMORY_AVAILABLE:
            return RegionAvailable;

        case MULTIBOOT_MEMORY_BADRAM:
            return RegionBad;

        case MULTIBOOT_MEMORY_NVS:
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
        case MULTIBOOT_MEMORY_RESERVED:
        default:
            return RegionReserved;
    }
}

void __stdcall MintLoaderProcessMultibootInformation(PMULTIBOOT_HEADER MultibootHeader) {
    SIZE_T Entry = 0;


    /* Process the Multiboot memory map */
    PMULTIBOOT_MMAP_ENTRY MmapEntry = (PMULTIBOOT_MMAP_ENTRY)(UINT64)MultibootHeader->MemoryMapBase;
    while ((PVOID)MmapEntry < (PVOID)(MultibootHeader->MemoryMapBase + MultibootHeader->MemoryMapLength)) {
        if (MmapEntry->Length == 0) {
            /* Invalid + marks end */
            break;
        }

        DEBUG("Multiboot memory descriptor (type: %i): %16x %16x\n", MmapEntry->Type, MmapEntry->Address, MmapEntry->Length);
    
        MemoryRegionList[Entry].MemoryType = MintLoaderMultibootToLoaderType(MmapEntry->Type);
        MemoryRegionList[Entry].Base = MmapEntry->Address;
        MemoryRegionList[Entry].Size = MmapEntry->Length;
        MemoryRegionList[Entry].NextRegion = &MemoryRegionList[Entry+1];
        
        if (Entry) {
            MemoryRegionList[Entry].PrevRegion = &MemoryRegionList[Entry-1];
        } else {
            MemoryRegionList[Entry].PrevRegion = NULL;
        }

        Entry++;

        /* Next entry */
        MmapEntry = (PMULTIBOOT_MMAP_ENTRY)((PVOID)MmapEntry + MmapEntry->Size + sizeof(MmapEntry->Size));
    }

    /* Zero last entry */
    MemoryRegionList[Entry-1].NextRegion = NULL;
}

void __stdcall MintLoaderMain(PMULTIBOOT_HEADER MultibootHeader, UINT32 MultibootMagic) {
    UiInit();
    UiPrint("MINTLDR v1.0\n");

    /* Initialize the serial port */
    SerialInitialize(1, 38400);
    UiPrint("COM1 initialized for debugging\n");

    /* Say hello */
    INFO("MINTLDR v1.0\n");

    /* Process Multiboot information */
    MintLoaderProcessMultibootInformation(MultibootHeader);
    UiPrint("Finished processing loader data\n");

    /* Initialize physical memory manager */
    MmInitializeMemoryMap(MemoryRegionList);
    UiPrint("Physical memory manager initialized\n");

    UINT_PTR Block = MmAllocatePhysicalPages(1);
    UiPrint("Got physical page: 0x%x\n", Block);
    UINT_PTR Block2 = MmAllocatePhysicalPages(2);
    UiPrint("Got physical page: 0x%x\n", Block2);
    MmFreePhysicalPages(Block, 1);
    UINT_PTR Block3 = MmAllocatePhysicalPages(5);
    UiPrint("Got physical page: 0x%x\n", Block3);
    UINT_PTR Block4 = MmAllocatePhysicalPages(1);
    UiPrint("Got physical page: 0x%x\n", Block4);

    for (;;);
}