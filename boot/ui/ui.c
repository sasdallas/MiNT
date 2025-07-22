/**
 * @file boot/ui/ui.c
 * @brief Bootstub UI
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <ui.h>

/* UI X/Y values */
int UiPositionX = 0;
int UiPositionY = 0;

/* UI colors */
GFX_COLOR UiForegroundColor = GFX_COLOR_WHITE;
GFX_COLOR UiBackgroundColor = GFX_COLOR_BLACK;

void UiInit() {
    UiClearScreen();
}

void UiSetColors(GFX_COLOR Foreground, GFX_COLOR Background) {
    UiForegroundColor = Foreground;
    UiBackgroundColor = Background;
}

void UiClearScreen() {
    GfxClear(UiBackgroundColor);
    UiPositionY = 0;
    UiPositionX = 0;
}

void UiPutCharacter(char c) {
    if (c == '\n') {
        UiPositionY++;
        UiPositionX = 0;
    } else {
        GfxPutCharacter(UiPositionX, UiPositionY, c, UiForegroundColor, UiBackgroundColor);
        UiPositionX++;
    }

    if (UiPositionX >= GfxScreenWidth) {
        UiPositionY++;
        UiPositionX = 0;
    }

    if (UiPositionY >= GfxScreenHeight) {
        UiClearScreen(); // TODO: Scrolling?
    }
}

int UiDefaultPrintCallback(PCHAR Str, SIZE_T Size) {
    for (unsigned i = 0; i < Size; i++) {
        UiPutCharacter(*Str);
    }

    return Size;
}

void UiPrintCallback(UiCallback Callback, PCHAR Format, va_list ap) {
    char *p = Format;

    while (*p) {
        // Temporary while I work on format specifiers
        Callback(p, 1);
        p++;
    }
}

void UiPrintVA(PCHAR Format, va_list ap) {
    UiPrintCallback(UiDefaultPrintCallback, Format, ap);
}

void UiPrint(PCHAR Format, ...) {
    va_list ap;
    va_start(ap, Format);
    UiPrintVA(Format, ap);
    va_end(ap);
}