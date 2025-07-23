/**
 * @file boot/arch/x86_64/mm.c
 * @brief x86_64 memory management subsystem
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <arch/x86_64/mm.h>
#include <mm/mm.h>
#include <dbg.h>
#include <ntdef.h>

extern UINT32 __mintldr_image_start;
extern UINT32 __mintldr_image_end;

UINT_PTR MmArchFindSpaceForPmmBitmap(SIZE_T SpaceRequired) {
    /* For now place the PMM buffer at the end. This isn't how you memory manage */
    DEBUG("MINTLDR image bounds: 0x%x - 0x%x\n", MM_PAGE_ALIGN_DOWN((UINT64)&__mintldr_image_start), MM_PAGE_ALIGN_UP(((UINT64)&__mintldr_image_end)));
    
    /* TODO: Validate this spot is available */
    UINT_PTR PmmBufferLocation = (UINT_PTR)MM_PAGE_ALIGN_UP((UINT64)&__mintldr_image_end);
    DEBUG("Placing PMM buffer at 0x%x\n", PmmBufferLocation);
    return PmmBufferLocation;
}