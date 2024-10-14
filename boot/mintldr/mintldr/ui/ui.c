/**
 * @file mintldr/ui/ui.c
 * @brief UI system - handles text output, popups, progress, colors, etc.
 * 
 * This file serves as a bridge between the graphics system, basically.
 * aka this is a glorified terminal
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
#include <ui/ui.h>
#include <vadefs.h>

static INT UI_CurrentX = 0;
static INT UI_CurrentY = 0;
static BYTE UI_ForegroundColor = COLOR_LIGHT_GRAY;
static BYTE UI_BackgroundColor = COLOR_BLACK;

/* Initialize UI system */
void
UiInit() {
    UI_CurrentX = 0;
    UI_CurrentY = 0;
    UI_ForegroundColor = COLOR_LIGHT_GRAY;
    UI_BackgroundColor = COLOR_BLACK;

    UiClearScreen();
}


/* Clear the screen */
void
UiClearScreen()
{
    GfxClearScreen(UI_ForegroundColor, UI_BackgroundColor);
    UI_CurrentY = 0;
    UI_CurrentX = 0;
}

/* Set colors */
void
UiSetColors(BYTE ForegroundColor, BYTE BackgroundColor) {
    UI_ForegroundColor = ForegroundColor;
    UI_BackgroundColor = BackgroundColor;
}

/* Put a character onto the screen */
void
UiPutCharacter(CHAR c) {

    /* Handle special characters */
    if (c == '\n')
    {
        /* Newline */
        UI_CurrentY += 1;
        UI_CurrentX = 0;
    }
    else if (c == '\t') 
    {
        /* Tab character */
        for (INT i = 0; i < 4; i += 1)
        {
            UiPutCharacter(' ');
        }
    }
    else {
        GfxPutChar(UI_CurrentX, UI_CurrentY, c, UI_ForegroundColor, UI_BackgroundColor);
        UI_CurrentX += 1;
    }

    if (UI_CurrentX >= VGA_WIDTH)
    {
        UI_CurrentY += 1;
        UI_CurrentX = 0;
    }

    if (UI_CurrentY >= VGA_HEIGHT)
    {
        UiClearScreen();
    }
} 

