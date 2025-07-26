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
#include <minilibc/stdlib.h>
#include <minilibc/string.h>

/* UI X/Y values */
int UiPositionX = 0;
int UiPositionY = 0;

/* UI colors */
GFX_COLOR UiForegroundColor = GFX_COLOR_LIGHT_GRAY;
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

void UiSetPosition(INT X, INT Y) {
    UiPositionX = X;
    UiPositionY = Y;
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
        UiPutCharacter(Str[i]);
    }

    return Size;
}

static PCHAR UiHexToString(unsigned __int64 Value, PCHAR Buffer, DWORD BufferSize)
{
    // Pointer to the end of the buffer
    PCHAR ptr = Buffer + BufferSize - 1;
    *ptr = '\0'; // null-terminate

    if (Buffer == 0) {
        *(--ptr) = '0';
    } else {
        while (Buffer != 0 && ptr > Buffer) {
            unsigned digit = Value & 0xF;
            *(--ptr) = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
            Value >>= 4;
        }
    }

    // Move result to start of buffer
    DWORD Length = (DWORD)(Buffer + BufferSize - 1 - ptr);
    memcpy(Buffer, ptr, Length + 1); // include null terminator

    return Buffer;
}


int UiPrintCallback(UiCallback Callback, PCHAR Format, va_list ap) {
    char *p = Format;

    SIZE_T CharactersWritten = 0;
    INT ArgumentWidth = 0;

    while (*p) {
        ArgumentWidth = 0;

        /* Check for format */
        if (*p != '%' || *(p+1) == '%') {
            if (*p == '%') p++;

            /* Calculate characters until next % */
            SIZE_T CharactersUntilFormat = 0;
            while (p[CharactersUntilFormat] && p[CharactersUntilFormat] != '%') {
                CharactersUntilFormat++;
            }

            /* Print out said characters */
            Callback(p, CharactersUntilFormat);
            
            p += CharactersUntilFormat;
            CharactersWritten += CharactersUntilFormat;
        
            continue;
        }

        /* We have a format, let's process it */
        const PCHAR FormatStart = p;
        p++;

        while (*p >= '0' && *p <= '9') {
            ArgumentWidth *= 10;
            ArgumentWidth += (INT)((*p) - '0');
            p++;
        }

        switch (*p) {
            /* %c - Character */
            case 'C':
            case 'c':
                p++;
                CHAR c = (CHAR)va_arg(ap, INT);

                Callback(&c, sizeof(c));
                CharactersWritten++;

                break;

            /* %s - String */
            case 'S':
            case 's':
                p++;
                CHAR* Str = va_arg(ap, char*);
                SIZE_T StrLength = strlen(Str);

                Callback(Str, StrLength);
                CharactersWritten += StrLength;

                break;

            /* %i - Integer */
            /* %x - Hexadecimal */
            case 'I':
            case 'i':
            case 'x':
            case 'X':
            case 'u':
            case 'U':
                /* HACK, as we don't support format specifiers like %16llX */
                ULONGLONG integer;
                if (ArgumentWidth > 8) {
                    integer = va_arg(ap, unsigned long long);
                } else {
                    integer = (ULONGLONG)va_arg(ap, long);
                }

                CHAR StringBuffer[32] = { 0 };
                if (*p == 'i') {
                    itoa(integer, StringBuffer, (*p == 'i') ? 10 : 16);
                } else {
                    /* Use custom function for bigger hex values */
                    UiHexToString(integer, StringBuffer, 16);
                }

                p++;

                StrLength = strlen(StringBuffer);
                
                if (ArgumentWidth && ArgumentWidth > StrLength) {
                    CHAR PaddingBuffer[32];
                    for (int i = 0; i < ArgumentWidth - StrLength; i++) {
                        PaddingBuffer[i] = '0';
                        PaddingBuffer[i+1] = 0;
                    }

                    Callback(PaddingBuffer, ArgumentWidth - StrLength);
                    CharactersWritten += ArgumentWidth - StrLength;
                }

                Callback(StringBuffer, StrLength);
                CharactersWritten += StrLength;
                
                break;

            default:
                /* Just print the rest out */
                p = FormatStart;
                StrLength = strlen(p);
                Callback(p, StrLength);
                CharactersWritten += StrLength;
                p += StrLength;
                break;
        }
    }

    return CharactersWritten;
}

int UiPrintVA(PCHAR Format, va_list ap) {
    return UiPrintCallback(UiDefaultPrintCallback, Format, ap);
}

int UiPrint(PCHAR Format, ...) {
    va_list ap;
    va_start(ap, Format);
    int r = UiPrintVA(Format, ap);
    va_end(ap);
    return r;
}  