/**
 * @file mintldr/include/gfx.h
 * @brief Graphics header file
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef GFX_H
#define GFX_H

/*** Screen details (+ colors) ***/
#ifdef _M_IX86
#include <arch/i386/gfx.h>
#endif

/*** Exposed functions ***/

/* Put a character */
void
GfxPutChar(int x, int y, char c, BYTE ForegroundColor, BYTE BackgroundColor);

/* Draw a rectangle */
void
GfxDrawRect(int x, int y, int width, int height, char c, BYTE ForegroundColor, BYTE BackgroundColor);

/* Clear the screen */
void
GfxClearScreen(BYTE ForegroundColor, BYTE BackgroundColor);

/* Draw a progress bar */
void
GfxDrawProgressBar(int x, int y, int width, int progress, BYTE foreground, BYTE background);

/* Draw a popup */
void
GfxDrawPopup(int x, int y, int width, int height, const char* title, const char* message);


#endif