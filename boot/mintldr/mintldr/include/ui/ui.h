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

#include <vadefs.h>

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

/* BAD HANDLER SYNTAX! */
typedef INT (*UiCallback)(PCSTR, SIZE_T);

/* UiPrint to a custom data object */
INT
UiPrintCallback(
    UiCallback Callback,
    PCSTR Format,
    va_list ap
);

/* UiPrint but it takes a va_list object */ 
INT
UiVAPrint(
    PCSTR Format,
    va_list ap
);

/* Set position */
VOID
UiSetPosition(
    INT XPos,
    INT YPos
);


/* Get Y */
INT
UiGetY();

/* Get X */
INT
UiGetX();



#endif