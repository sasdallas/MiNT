/**
 * @file boot/include/arch/x86_64/mm.h
 * @brief Memory management definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_ARCH_X86_64_MM_H__
#define __MINTLDR_ARCH_X86_64_MM_H__ 1

__MINT_BEGIN_DECLS

#include <winnt.h>

#define MM_PAGE_SIZE            4096
#define MM_PAGE_SHIFT           12
#define MM_PAGE_MASK            0xFFF

#define MM_PML4_INDEX(x) (((x) >> (MM_PAGE_SHIFT + 27)) & 0x1FF)
#define MM_PDPT_INDEX(x) (((x) >> (MM_PAGE_SHIFT + 18)) & 0x1FF)
#define MM_PD_INDEX(x) (((x) >> (MM_PAGE_SHIFT + 9)) & 0x1FF)
#define MM_PT_INDEX(x) (((x) >> (MM_PAGE_SHIFT)) & 0x1FF)

typedef struct MEMORY_PAGE {
    union {
        struct {
            UINT64 Present:1;
            UINT64 Writable:1;
            UINT64 Usermode:1;
            UINT64 Writethrough:1;
            UINT64 CacheDisable:1;
            UINT64 Accessed:1;
            UINT64 Dirty:1;
            UINT64 Size:1;
            UINT64 Global:1;
            UINT64 Available:3;
            UINT64 Address:28;
            UINT64 Reserved:12;
            UINT64 Available2:11;
            UINT64 NoExecute:1;
        };

        UINT64 Raw;
    }; 
} __attribute__((packed)) MEMORY_PAGE, *PMEMORY_PAGE;


#define MM_PHYSICAL_MAP_REGION      (UINT64)0xFFFFFF8000000000
#define MM_PHYSICAL_MAP_SIZE        (UINT64)0x1000000000
#define MM_PAGE_SIZE_LARGE          (UINT64)0x200000

__MINT_END_DECLS

#endif