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

#define VGA_COLOR(fg, bg) (fg | bg << 4)
#define VGA_ENTRY(ch, fg, bg) ((unsigned short)(ch) | ((unsigned short)VGA_COLOR(fg, bg) << 8))

SIZE_T GfxScreenWidth = VGA_WIDTH;
SIZE_T GfxScreenHeight = VGA_HEIGHT;

VOID GfxClear(GFX_COLOR Color) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            GfxPutCharacter(x, y, ' ', GFX_COLOR_WHITE, Color);
        }
    }
}

VOID GfxPutCharacter(int x, int y, char c, GFX_COLOR Foreground, GFX_COLOR Background) {
    *(PUSHORT)(VGA_MEMORY + ((y * VGA_WIDTH + x)*2)) = VGA_ENTRY(c, Foreground, Background);
    return;
}