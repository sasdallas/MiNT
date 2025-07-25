/**
 * @file boot/ldr/ldr.c
 * @brief PE loader
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <ldr/ldr.h>
#include <dbg.h>

INT LdrImageLoad(UINT_PTR Base, MINTLDR_MEMORY_TYPE MemoryType, PVOID *ImageBase) {
    PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)Base;
    
    /* Validate MZ magic */
    if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        ERROR("DOS signature not matched: %4x\n", DosHeader->e_magic);
        return 1;
    }

    return 1;
}