/**
 * @file boot/arch/x86_64/ldr.c
 * @brief Ldr module lookup code
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <ldr/ldr.h>
#include <arch/x86_64/main.h>
#include <minilibc/string.h>
#include <dbg.h>

UINT_PTR LdrArchImageLookup(PCHAR ImageName) {
    DEBUG("%s\n", ImageName);
    /* Lookup the module based off the image name */
    for (SIZE_T i = 0; i < ModuleCount; i++) {
        if (ModuleList[i].Cmdline) {
            DEBUG("Check %s\n", ModuleList[i].Cmdline);
            if (!strcmp(ModuleList[i].Cmdline, ImageName)) {
                return ModuleList[i].Base;
            }
        }
    }

    /* Didn't find it */
    return NULL;
}