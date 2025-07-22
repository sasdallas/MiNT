/**
 * @file boot/arch/x86_64/main.c
 * @brief MiNT loader main
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <winnt.h>
#include <ntdef.h>
#include <ui.h>

void __cdecl MintLoaderMain() {
    GfxPutCharacter(0, 0, 'c', GFX_COLOR_BLACK, GFX_COLOR_WHITE);
    for (;;);
}