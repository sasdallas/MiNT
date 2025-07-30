/**
 * @file boot/include/mm/region.h
 * @brief Memory region
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_MM_REGION_H__
#define __MINTLDR_MM_REGION_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>

typedef enum {
    RegionAvailable,    /* This is the only reason you can initialize! */
    RegionReserved,
    RegionBad,
    RegionLoaderCode,
    RegionLoaderData,
    RegionLoaderHeap,
    RegionKernel,
    RegionDll,
    RegionKernelStack,
    RegionKernelParameterBlock,
} MINTLDR_MEMORY_TYPE;

typedef struct MINTLDR_MEMORY_REGION {
    MINTLDR_MEMORY_TYPE                 MemoryType;         // Memory type
    UINT_PTR                            Base;               // Base address
    UINT_PTR                            Size;               // Size

    struct MINTLDR_MEMORY_REGION*       NextRegion;         // Next region
    struct MINTLDR_MEMORY_REGION*       PrevRegion;         // Previous region
} MINTLDR_MEMORY_REGION, *PMINTLDR_MEMORY_REGION;

__MINT_END_DECLS

#endif