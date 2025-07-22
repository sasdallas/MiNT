/**
 * @file boot/include/ui.h
 * @brief UI library for bootloader
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#ifndef __MINTLDR_UI_H__
#define __MINTLDR_UI_H__ 1

#include <arch/graphics.h>
#include <ntdef.h>
#include <vadefs.h>

typedef INT (*UiCallback)(PCHAR, SIZE_T);

void UiInit();
void UiSetColors(GFX_COLOR Foreground, GFX_COLOR Background);
void UiPutCharacter(char c);
int UiPrintCallback(UiCallback Callback, PCHAR Format, va_list ap);
int UiPrintVA(PCHAR Format, va_list ap);
int UiPrint(PCHAR Format, ...);
void UiClearScreen();

#endif