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
#include <ldr/ldr.h>

/* Memory region list */
MINTLDR_MEMORY_REGION MemoryRegionList[81];
SIZE_T ModuleCount = 0;

/* Module list */
MINTLDR_MODULE ModuleList[80];

/* The kernel module (ugh) */
PMINTLDR_MODULE MintKernelModule = NULL;


extern UINT32 __mintldr_image_start;
extern UINT32 __mintldr_image_end;

/* MINTLDR image bounds */
UINT_PTR MintLoaderImageStart;
UINT_PTR MintLoaderImageEnd;

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
        case ModuleHal:
            return "HalImage";
        case ModuleKdcom:
            return "KdcomImage";
        case ModuleDll:
            return "GenericDll";
        case ModuleRamdisk:
            return "LdrRamdisk";
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
    while ((PVOID)Module < (PVOID)(UINT64)(MultibootHeader->ModuleStart + (MultibootHeader->ModuleCount * sizeof(MULTIBOOT_MODULE)))) {
        ModuleList[ModuleCount].PhysicalPageBase = MM_PAGE_ALIGN_DOWN(Module->ModuleStart);
        ModuleList[ModuleCount].Size = MM_PAGE_ALIGN_UP(Module->ModuleEnd - Module->ModuleStart);

        /* Check command line for type */
        PCHAR Cmdline = (PCHAR)Module->ModuleCmdline;

        /* Increase image bounds to encompass module */
        if (MM_PAGE_ALIGN_UP(Module->ModuleEnd) > MintLoaderImageEnd) {
            MintLoaderImageEnd = MM_PAGE_ALIGN_UP(Module->ModuleEnd);
        }
        
        /* !!! */   
        if (!strcmp(Cmdline, "kernel")) {
            INFO("Found MINTKRNL.EXE: %16x - %16x\n", Module->ModuleStart, Module->ModuleEnd);
            ModuleList[ModuleCount].Type = ModuleKernel;
        } else if (!strcmp(Cmdline, "hal.dll")) {
            INFO("Found HAL.DLL: %16x - %16x\n", Module->ModuleStart, Module->ModuleEnd);
            ModuleList[ModuleCount].Type = ModuleHal;
        } else {
            WARN("Unrecognized Multiboot module of type: %s\n", Cmdline);
            ModuleList[ModuleCount].Type = ModuleUnknown;
        }
        
        ModuleList[ModuleCount].Cmdline = Cmdline;

        ModuleCount++;
        Module = (PMULTIBOOT_MODULE)((PVOID)Module + sizeof(MULTIBOOT_MODULE));
    }

    /* TEMP: Make sure we got kernel */
    for (SIZE_T i = 0; i < ModuleCount; i++) {
        if (ModuleList[i].Type == ModuleKernel) {
            MintKernelModule = &ModuleList[i];
        }

        DEBUG("Multiboot module: %16x - %16x: %s\n", ModuleList[i].PhysicalPageBase, ModuleList[i].PhysicalPageBase + ModuleList[i].Size, ModuleTypeToString(ModuleList[i].Type));
    }

    if (!MintKernelModule) {
        MintBugCheckWithMessage(FILE_NOT_FOUND, "Missing MINTKRNL.EXE or it was not passed to MINTLDR");
    }
}

void MintLoaderRelocateModules() {
    for (SIZE_T i = 0; i < ModuleCount; i++) {
        PMINTLDR_MEMORY_REGION Region;
        MmAllocateRegion(ModuleList[i].Size / MM_PAGE_SIZE, &Region);
        ModuleList[i].Base = Region->Base;
        Region->MemoryType = RegionLoaderData;

        /* Setup each page in the module */
        for (UINT_PTR v = 0x0; v < ModuleList[i].Size; v += MM_PAGE_SIZE) {
            MINTLDR_PAGE Page = {
                .Flags = MINTLDR_PAGE_PRESENT,
                .Address = ModuleList[i].PhysicalPageBase + v
            };

         
            MmArchSetPage(&Page, ModuleList[i].Base + v);
        }

        INFO("Copied module %s to %16x - %16x (%s)\n", ModuleTypeToString(ModuleList[i].Type), ModuleList[i].Base, ModuleList[i].Base + ModuleList[i].Size, ModuleList[i].Cmdline);

        /* Relocate the module cmdline */
        SIZE_T Len = strlen(ModuleList[i].Cmdline);
        if (Len) {
            PVOID NewCmdline = MmAllocateHeap(MintDefaultHeap, Len+1);
            memcpy(NewCmdline, ModuleList[i].Cmdline, Len);
            ((PCHAR)NewCmdline)[Len] = 0;
            ModuleList[i].Cmdline = NewCmdline;
        } else {
            ModuleList[i].Cmdline = NULL;
        }
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

    /* Initialize MINTLDR bounds */
    MintLoaderImageStart = MM_PAGE_ALIGN_DOWN((UINT64)&__mintldr_image_start);
    MintLoaderImageEnd = MM_PAGE_ALIGN_UP((UINT64)&__mintldr_image_end);

    /* Process Multiboot information and update bounds */
    MintLoaderProcessMultibootInformation(MultibootHeader);
    UiPrint("Finished processing loader data\n");

    /* Initialize physical memory manager */
    MmInitializeMemoryMap(MemoryRegionList);
    UiPrint("Physical memory manager initialized\n");

    /* Initialize virtual memory manager */
    MmInitializeMemoryManager();
    UiPrint("Memory manager online\n");

    /* Relocate modules */
    MintLoaderRelocateModules();
    UiPrint("Modules copied to memory\n");

    /* Load kernel */
    PMINTLDR_LOADED_IMAGE KernelImage = NULL;
    if (LdrImageLoadEx("mintkrnl.exe", MintKernelModule->Base, RegionKernel, NULL, &KernelImage) || !KernelImage) {
        MintBugCheck(KERNEL_IMAGE_CORRUPT);
    }

    /* Create loader param block */
    PLOADER_PARAMETER_BLOCK LoaderBlock;
    LdrImageCreateLoaderBlock(KernelImage, &LoaderBlock);

    /* Away we go! */
    UiPrint("Launching MINTKRNL\n");
    ((void (*)(PLOADER_PARAMETER_BLOCK))(KernelImage->Entrypoint))(LoaderBlock);

    for (;;);
}