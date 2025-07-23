/**
 * @file boot/include/mm/mm.h
 * @brief MINTLDR memory management subsystem
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_MM_MM_H__
#define __MINT_MM_MM_H__ 1

__MINT_BEGIN_DECLS

#include <mm/region.h>
#include <msvctarget.h>
#include <winnt.h>

#ifdef _M_AMD64
#include <arch/x86_64/mm.h>
#else
#error "Unknown architecture"
#endif

#define MM_PAGE_ALIGN_UP(address) (((address) % MM_PAGE_MASK) ? (((address) + MM_PAGE_SIZE) & ~MM_PAGE_MASK) : (address))
#define MM_PAGE_ALIGN_DOWN(address) (((address) & ~MM_PAGE_MASK))

/* PMM */
INT                     MmInitializeMemoryMap(PMINTLDR_MEMORY_REGION MapStart);
UINT_PTR                MmAllocatePhysicalPages(SIZE_T PageCount);
INT                     MmFreePhysicalPages(UINT_PTR Pages, SIZE_T PageCount);
PMINTLDR_MEMORY_REGION  MmGetPhysicalMemoryMap();

/* VMM */
UINT_PTR                MmAllocatePages(SIZE_T PageCount);
INT                     MmFreePages(SIZE_T PageCount);

/* The architecture should implement these */
UINT_PTR                MmArchFindSpaceForPmmBitmap(SIZE_T SpaceRequired);

__MINT_END_DECLS

#endif