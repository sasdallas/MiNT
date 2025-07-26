/**
 * @file boot/mm/vmm.c
 * @brief Virtual memory manager
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <mm/mm.h>
#include <winnt.h>
#include <bugcheck.h>
#include <dbg.h>

/* Memory region list */
PMINTLDR_MEMORY_REGION MintMemoryRegionList = NULL;

/* Initial MINTLDR heap */
MINTLDR_HEAP MintInitialHeap = { 0 };

#define MINTLDR_INITIAL_HEAP_PAGES          32

static PCHAR MmMemoryTypeToString(MINTLDR_MEMORY_TYPE Type) {
    switch (Type) {
        case RegionAvailable:
            return "Available";
        case RegionReserved:
            return "Reserved";
        case RegionBad:
            return "BadMemory";
        case RegionLoaderCode:
            return "LoaderCode";
        case RegionLoaderData:
            return "LoaderData";
        case RegionLoaderHeap:
            return "LoaderHeap";
        case RegionKernel:
            return "KernelImage";
        case RegionDll:
            return "DllFile";
        default:
            return "Unknown";
    }
}

VOID MmDbgPrintMemoryMap() {
    DEBUG("MINTLDR memory region list:\n");
    PMINTLDR_MEMORY_REGION Region = MintMemoryRegionList;
    while (Region) {
        DEBUG("Region: %16x - %16x: %s\n", Region->Base, Region->Base + Region->Size, MmMemoryTypeToString(Region->MemoryType));

        if (!Region->NextRegion) break;
        Region = Region->NextRegion;
    }
    
}

INT MmInitializeMemoryManager() {
    /* Setup the architecture's page map */
    MmArchSetupPageMap();

    /* Setup the initial MINTLDR heap */
    MintInitialHeap.Base = MmArchFindSpaceForInitialHeap(MINTLDR_INITIAL_HEAP_PAGES * MM_PAGE_SIZE);
    if (!MintInitialHeap.Base) {
        MintBugCheckWithMessage(MEMORY_NO_SPACE, "No space for initial heap");
    }

    MintInitialHeap.Type = RegionLoaderHeap;
    MintInitialHeap.Size = MINTLDR_INITIAL_HEAP_PAGES * MM_PAGE_SIZE;

    /* Initialize the heap */
    MmInitHeap(&MintInitialHeap);
    DEBUG("Initial heap initialized to 0x%16x - 0x%16x\n", MintInitialHeap.Base, MintInitialHeap.Base + MintInitialHeap.Size);

    /* Unmark the MINTLDR image in the region map */
    MmArchUnmarkMintldrImage();

    /* Unmark the initial heap */
    MmCreateNewRegion(RegionLoaderHeap, MintInitialHeap.Base, MintInitialHeap.Size);

    MmCreateNewRegion(RegionBad, 0x0, MM_PAGE_SIZE);

    /* Start the default heap */
    MmInitDefaultHeap();

    /* Debug */
    MmDbgPrintMemoryMap();

    return 0;
}

INT MmCreateNewRegion(MINTLDR_MEMORY_TYPE Type, UINT_PTR Address, SIZE_T Size) {
    /* Find out where we need to put this region */
    PMINTLDR_MEMORY_REGION Region = MintMemoryRegionList;
    while (Region) {
        if (!Region->NextRegion) break;
        
        if (Region->Base > Address) {
            /* Don't panic, maybe this is the first region? */
            if (Region == MintMemoryRegionList) {
                /* Is there enough space? */
                if (Address + Size <= Region->Base) {
                    /* Yes, we can put a region here */
                    PMINTLDR_MEMORY_REGION NewRegion = MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));
                    NewRegion->Base = Address;
                    NewRegion->MemoryType = Type;
                    NewRegion->NextRegion = MintMemoryRegionList;
                    NewRegion->PrevRegion = NULL;
                    NewRegion->Size = Size;

                    Region->PrevRegion = NewRegion;
                    MintMemoryRegionList = NewRegion;
                    return 0;
                }
            }

            MintBugCheckWithMessage(MEMORY_REGION_LIST_CORRUPT, "Region collision. This is a bug");
        }

        /* Check the next region */
        PMINTLDR_MEMORY_REGION Next = Region->NextRegion;
        UINT_PTR End = Region->Base + Region->Size;
        ASSERT_BUGCHECK_SIMPLE(Next->Base >= Region->Base + Region->Size, MEMORY_REGION_LIST_CORRUPT);
        SIZE_T HoleSize = Next->Base - (Region->Base + Region->Size);

        if (HoleSize >= Size && Address > End && Address + Size < Next->Base) {
            /* We have space in between these two areas */
            PMINTLDR_MEMORY_REGION NewRegion = MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));
            NewRegion->Base = Address;
            NewRegion->MemoryType = Type;
            NewRegion->NextRegion = Next;
            NewRegion->PrevRegion = Region;
            NewRegion->Size = Size;

            Next->PrevRegion = NewRegion;
            Region->NextRegion = NewRegion;
            return 0;
        }
        
        Region = Region->NextRegion;
    }

    /* Create a new region and append it to the back of the memory system */
    PMINTLDR_MEMORY_REGION NewRegion = MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));
    NewRegion->Base = Address;
    NewRegion->MemoryType = Type;
    NewRegion->NextRegion = NULL;
    NewRegion->PrevRegion = Region;
    NewRegion->Size = Size;

    if (!Region) {
        MintMemoryRegionList = NewRegion;
    } else {
        /* Region should be the last region */
        Region->NextRegion = NewRegion;
    }

    return 0;
}

PMINTLDR_MEMORY_REGION MmGetRegion(UINT_PTR Base) {
    PMINTLDR_MEMORY_REGION Region = MintMemoryRegionList;
    while (Region) {
        if (Base >= Region->Base && Base <= Region->Base + Region->Size) return Region;
        Region = Region->NextRegion;
    }

    return NULL;
}

INT MmAllocateRegion(SIZE_T PageCount, PMINTLDR_MEMORY_REGION *RegionOut) {
    if (!PageCount) {
        WARN("MmAllocatePagesEx called for 0 pages\n");
        return NULL;
    }

    /* Get total size needed */
    SIZE_T Size = PageCount * MM_PAGE_SIZE;

    /* Find holes to place pages */
    PMINTLDR_MEMORY_REGION Region = MintMemoryRegionList;

    /* Check for big enough holes */
    if (Region->Base > Size) {
        /* We have enough space to put one at the beginning */
        PMINTLDR_MEMORY_REGION NewRegion = MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));
        NewRegion->Base = 0x0;
        NewRegion->MemoryType = RegionLoaderData;
        NewRegion->NextRegion = Region;
        NewRegion->PrevRegion = NULL;
        Region->PrevRegion = NewRegion;
        NewRegion->Size = Size;
        MintMemoryRegionList = NewRegion;
        if (RegionOut) *RegionOut = NewRegion;
        return 0;
    }

    while (Region) {
        if (!Region->NextRegion) break;
        PMINTLDR_MEMORY_REGION Next = Region->NextRegion;

        /* Check for hole size */
        ASSERT_BUGCHECK_SIMPLE(Next->Base >= Region->Base + Region->Size, MEMORY_REGION_LIST_CORRUPT);
        SIZE_T HoleSize = Next->Base - (Region->Base + Region->Size);
        UINT_PTR End = Region->Base + Region->Size;

        if (HoleSize > Size) {
            /* We have space in between these two areas */
            PMINTLDR_MEMORY_REGION NewRegion = MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));
            NewRegion->Base = End;
            NewRegion->MemoryType = RegionLoaderData;
            NewRegion->NextRegion = Next;
            NewRegion->PrevRegion = Region;
            NewRegion->Size = Size;

            Next->PrevRegion = NewRegion;
            Region->NextRegion = NewRegion;
            if (RegionOut) *RegionOut = NewRegion;
            return 0;
        } 

        Region = Region->NextRegion;
    }

    /* Create a new region and append it to the back of the memory system */
    PMINTLDR_MEMORY_REGION NewRegion = MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));
    NewRegion->Base = Region->Base + Region->Size;
    NewRegion->MemoryType = RegionLoaderData;
    NewRegion->NextRegion = NULL;
    NewRegion->PrevRegion = Region;
    NewRegion->Size = Size;
    Region->NextRegion = NewRegion;
    if (RegionOut) *RegionOut = NewRegion;
    return 0;
}

UINT_PTR MmAllocatePagesAtAddress(SIZE_T PageCount, UINT_PTR Base, PMINTLDR_MEMORY_REGION *RegionOut) {
    if (MmCreateNewRegion(RegionLoaderHeap, Base, PageCount * MM_PAGE_SIZE)) {
        ERROR("MmCreateNewRegion failed\n");
        return NULL;
    }

    PMINTLDR_MEMORY_REGION Region = MmGetRegion(Base);

    /* Now allocate each part of the page */
    for (UINT_PTR i = Region->Base; i < Region->Base + Region->Size; i += MM_PAGE_SIZE) {
        MINTLDR_PAGE Page = {
            .Flags = MINTLDR_PAGE_PRESENT | MINTLDR_PAGE_WRITABLE,
            .Address = MmAllocatePhysicalPages(1)
        };

        MmArchSetPage(&Page, i);
    }

    if (RegionOut) *RegionOut = Region;
    return Region->Base;
}

UINT_PTR MmAllocatePagesEx(SIZE_T PageCount, PMINTLDR_MEMORY_REGION *RegionOut) {
    PMINTLDR_MEMORY_REGION Region;
    if (MmAllocateRegion(PageCount, &Region)) {
        ERROR("MmAllocateRegion failed\n");
        return NULL; /* TODO: Error handling? */
    }

    /* Now allocate each part of the page */
    for (UINT_PTR i = Region->Base; i < Region->Base + Region->Size; i += MM_PAGE_SIZE) {
        MINTLDR_PAGE Page = {
            .Flags = MINTLDR_PAGE_PRESENT | MINTLDR_PAGE_WRITABLE,
            .Address = MmAllocatePhysicalPages(1)
        };

        MmArchSetPage(&Page, i);
    }

    if (RegionOut) *RegionOut = Region;
    return Region->Base;
}

UINT_PTR MmAllocatePages(SIZE_T PageCount) {
    return MmAllocatePagesEx(PageCount, NULL);
}

INT MmFreePages(UINT_PTR Base, SIZE_T PageCount) {
    ERROR("MmFreePages: TODO\n");
    return 0;
}