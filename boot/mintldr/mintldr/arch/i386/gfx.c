/**
 * @file mintldr/arch/i386/gfx.c
 * @brief I386 MiNT graphics library
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <mintldr.h>
#include <gfx.h>
#include <arch/i386/gfx.h>


PWORD VGA_BUFFER = (WORD*)VGA_MEMORY;

/* Put a character */
void
GfxPutChar(int x, int y, char c, BYTE ForegroundColor, BYTE BackgroundColor)
{
    int index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = (WORD)c | (WORD)((ForegroundColor | BackgroundColor << 4) << 8);
}


/* Draw a rectangle */
void
GfxDrawRect(int x, int y, int width, int height, char c, BYTE ForegroundColor, BYTE BackgroundColor)
{
    for (int j = y; j < y + height; j++)
    {
        for (int i = x; i < x + width; i++)
        {
            GfxPutChar(i, j, c, ForegroundColor, BackgroundColor);
        }
    }
}


/* Clear the screen */
void
GfxClearScreen(BYTE ForegroundColor, BYTE BackgroundColor) {

    GfxDrawRect(0, 0, VGA_WIDTH, VGA_HEIGHT, ' ', ForegroundColor, BackgroundColor);
}

/* Draw a progress bar */
void
GfxDrawProgressBar(int x, int y, int width, int progress, BYTE foreground, BYTE background)
{
    int filledWidth = (progress * width) / 100;
    
    // Draw empty bar
    GfxDrawRect(x, y, width, 1, ' ', foreground, background);
    
    // Draw filled portion
    GfxDrawRect(x, y, filledWidth, 1, 219, foreground, background);
}


/* Draw popup across the screen */
void
GfxDrawPopup(int x, int y, int width, int height, const char* title, const char* message)
{
    // Draw popup background
    GfxDrawRect(x, y, width, height, ' ', COLOR_BLACK, COLOR_LIGHT_GRAY);
    
    // Draw title bar
    GfxDrawRect(x, y, width, 1, ' ', COLOR_WHITE, COLOR_BLUE);
    
    // Draw title text
    int titleLen = 0;
    while (title[titleLen] != '\0' && titleLen < width - 2)
    {
        GfxPutChar(x + 1 + titleLen, y, title[titleLen], COLOR_WHITE, COLOR_BLUE);
        titleLen++;
    }
    
    // Draw message
    int msgLen = 0;
    int curX = x + 1;
    int curY = y + 2;
    while (message[msgLen] != '\0' && curY < y + height - 1)
    {
        if (curX >= x + width - 1 || message[msgLen] == '\n')
        {
            curX = x + 1;
            curY++;
        }
        else
        {
            GfxPutChar(curX, curY, message[msgLen], COLOR_BLACK, COLOR_LIGHT_GRAY);
            curX++;
        }
        msgLen++;
    }
}

