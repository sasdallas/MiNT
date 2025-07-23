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

/* Page flags */
#define MINTLDR_PAGE_PRESENT                0x1
#define MINTLDR_PAGE_WRITABLE               0x2
#define MINTLDR_PAGE_USERMODE               0x4

/* Generic MINTLDR page */
typedef struct MINTLDR_PAGE {
    UINT8           Flags;          /* Page flags */
    UINT_PTR        Address;        /* Page frame */
} MINTLDR_PAGE, *PMINTLDR_PAGE;

/* PMM */
INT                     MmInitializeMemoryMap(PMINTLDR_MEMORY_REGION MapStart);
UINT_PTR                MmAllocatePhysicalPages(SIZE_T PageCount);
INT                     MmFreePhysicalPages(UINT_PTR Pages, SIZE_T PageCount);
PMINTLDR_MEMORY_REGION  MmGetPhysicalMemoryMap();
INT                     MmMarkPhysicalRegion(PMINTLDR_MEMORY_REGION Region);

/* VMM */
INT                     MmInitializeMemoryManager();
UINT_PTR                MmAllocatePages(SIZE_T PageCount);
INT                     MmFreePages(SIZE_T PageCount);
INT                     MmCreateNewAllocation(UINT_PTR Address, SIZE_T Size);
INT                     MmSetAllocationType(UINT_PTR Address, MINTLDR_MEMORY_TYPE Type);

/* The architecture should implement these */
UINT_PTR                MmArchFindSpaceForPmmBitmap(SIZE_T SpaceRequired);
VOID                    MmArchUnmarkMintldrImagePhysical();
VOID                    MmArchUnmarkMintldrImage();
INT                     MmArchSetupPageMap();
INT                     MmArchGetPage(PMINTLDR_PAGE Page, UINT_PTR Address);
INT                     MmArchSetPage(PMINTLDR_PAGE Page, UINT_PTR Address);

__MINT_END_DECLS

#endif