/**
 * @file boot/include/arch/graphics.h
 * @brief Graphics header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_ARCH_GRAPHICS_H__
#define __MINTLDR_ARCH_GRAPHICS_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>

typedef enum {
    GFX_COLOR_BLACK,
    GFX_COLOR_BLUE,
    GFX_COLOR_GREEN,
    GFX_COLOR_CYAN,
    GFX_COLOR_RED,
    GFX_COLOR_MAGENTA,
    GFX_COLOR_BROWN,
    GFX_COLOR_LIGHT_GRAY,
    GFX_COLOR_DARK_GRAY,
    GFX_COLOR_LIGHT_BLUE,
    GFX_COLOR_LIGHT_GREEN,
    GFX_COLOR_LIGHT_CYAN,
    GFX_COLOR_LIGHT_RED,
    GFX_COLOR_LIGHT_MAGENTA,
    GFX_COLOR_YELLOW,
    GFX_COLOR_WHITE
} GFX_COLOR;

extern SIZE_T GfxScreenWidth;
extern SIZE_T GfxScreenHeight;

VOID GfxClear(GFX_COLOR Color);
VOID GfxPutCharacter(int x, int y, char c, GFX_COLOR Foreground, GFX_COLOR Background);

__MINT_END_DECLS

#endif