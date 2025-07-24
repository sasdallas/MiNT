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
#include <arch/x86_64/main.h>
#include <minilibc/string.h>

/* Memory region list */
MINTLDR_MEMORY_REGION MemoryRegionList[81];

/* Module list */
MINTLDR_MODULE ModuleList[80];

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

static PCHAR ModuleTypeToString(MINTLDR_MODULE_TYPE Type) {
    switch (Type) {
        case ModuleKernel:
            return "KernelImage";
        default:
            return "Unknown";
    }
}

VOID __stdcall MintLoaderProcessMultibootInformation(PMULTIBOOT_HEADER MultibootHeader) {
    SIZE_T Entry = 0;


    /* Process the Multiboot memory map */
    PMULTIBOOT_MMAP_ENTRY MmapEntry = (PMULTIBOOT_MMAP_ENTRY)(UINT64)MultibootHeader->MemoryMapBase;
    while ((PVOID)MmapEntry < (PVOID)(UINT64)(MultibootHeader->MemoryMapBase + MultibootHeader->MemoryMapLength)) {
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

    /* Now get each Multiboot entry */
    PMULTIBOOT_MODULE Module = (PMULTIBOOT_MODULE)(UINT64)MultibootHeader->ModuleStart;
    SIZE_T ModIndex = 0;
    while ((PVOID)Module < (PVOID)(UINT64)(MultibootHeader->ModuleStart + MultibootHeader->ModuleCount)) {
        ModuleList[ModIndex].PhysicalPageBase = Module->ModuleStart;
        ModuleList[ModIndex].ModuleSize = Module->ModuleEnd - Module->ModuleStart;

        /* Check command line for type */
        PCHAR Cmdline = (PCHAR)Module->ModuleCmdline;

        /* !!! */   
        if (!strcmp(Cmdline, "kernel")) {
            INFO("Found MINTKRNL.EXE: %16x - %16x\n", Module->ModuleStart, Module->ModuleEnd);
            ModuleList[ModIndex].Type = ModuleKernel;
        } else {
            WARN("Unrecognized Multiboot module of type: %s\n", Cmdline);
            ModuleList[ModIndex].Type = ModuleUnknown;
        }

        ModIndex++;
        Module = (PMULTIBOOT_MODULE)((PVOID)Module + sizeof(MULTIBOOT_MODULE));
    }

    /* TEMP: Make sure we got kernel */
    INT FoundKernel = 0;
    for (SIZE_T i = 0; i < ModIndex; i++) {
        if (ModuleList[i].Type == ModuleKernel) {
            FoundKernel = 1;
        }

        DEBUG("Multiboot module: %16x - %16x: %s\n", ModuleList[i].PhysicalPageBase, ModuleList[i].PhysicalPageBase + ModuleList[i].ModuleSize, ModuleTypeToString(ModuleList[i].Type));
    }

    if (!FoundKernel) {
        MintBugCheckWithMessage(FILE_NOT_FOUND, "Missing MINTKRNL.EXE or it was not passed to MINTLDR");
    }
}

void __stdcall MintLoaderMain(PMULTIBOOT_HEADER MultibootHeader, UINT32 MultibootMagic) {
    UiInit();
    UiSetColors(GFX_COLOR_BLACK, GFX_COLOR_LIGHT_GRAY);
    UiPrint("MINTLDR v1.0                                                                  \n");

    UiSetPosition(0, 24);
    UiPrint("Starting MiNT                                                                 ");
    UiSetPosition(0, 1);

    UiSetColors(GFX_COLOR_LIGHT_GRAY, GFX_COLOR_BLACK);

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

    /* Initialize virtual memory manager */
    MmInitializeMemoryManager();
    UiPrint("Memory manager online\n");

    UiPrint("Getting ready to load MiNT\n");
    MintBugCheckWithMessage(OUT_OF_MEMORY, "MiNTy Fresh\n");

    for (;;);
}