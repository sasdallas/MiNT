/**
 * @file mintldr/include/ui/ui.h
 * @brief UI include file
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef UI_H
#define UI_H

/* Initialize UI system */
void
UiInit();

/* Clear the screen */
void
UiClearScreen();

/* Set colors */
void
UiSetColors(BYTE ForegroundColor, BYTE BackgroundColor);

/* Put a character onto the screen */
void
UiPutCharacter(CHAR c);

/* printf() like function */
INT
UiPrint(
    PCSTR Format, ...
);


#endif