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

    DEBUG("Initial heap initialized to 0x%16x - 0x%16x, first big block at %16x\n", MintInitialHeap.Base, MintInitialHeap.Base + MintInitialHeap.Size, MintInitialHeap.Head);

    MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));

    MmAllocateHeap(&MintInitialHeap, sizeof(MINTLDR_MEMORY_REGION));

    return 0;
}

