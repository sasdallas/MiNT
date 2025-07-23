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

/* Memory region list */
PMINTLDR_MEMORY_REGION MintMemoryRegionList = NULL;

/* Initial count of memory regions */
#define MINTLDR_MEMORY_REGION_COUNT             20

INT MmInitializeMemoryManager() {
    /* Setup the architecture's page map */
    MmArchSetupPageMap();

    return 0;
}

