/**
 * @file boot/arch/x86_64/graphics.c
 * @brief (VGA) graphics subsystem
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <arch/graphics.h>
#include <basetsd.h>

#define VGA_MEMORY          0xB8000
#define VGA_WIDTH           80
#define VGA_HEIGHT          25
#define VGA_ENTRY(ch, fg, bg) (((UINT16)(ch) | ((UINT16)((fg) | (bg) << 4)) << 8))

SIZE_T GfxScreenWidth = VGA_WIDTH;
SIZE_T GfxScreenHeight = VGA_HEIGHT;
PUINT16 VGA_BUFFER = (PUINT16)VGA_MEMORY;

VOID GfxClear(GFX_COLOR Color) {
    for (int y = 0; y < GfxScreenHeight; y++) {
        for (int x = 0; x < GfxScreenWidth; x++) {
            VGA_BUFFER[y * VGA_WIDTH + x] = VGA_ENTRY(' ', GFX_COLOR_WHITE, Color);
        }
    }
}

VOID GfxPutCharacter(int x, int y, char c, GFX_COLOR Foreground, GFX_COLOR Background) {
    VGA_BUFFER[y * VGA_WIDTH + x] = VGA_ENTRY(c, Foreground, Background);
}