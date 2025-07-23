/**
 * @file boot/ui/bugcheck.c
 * @brief Bugcheck interface
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <bugcheck.h>
#include <ui.h>

PCHAR MintBugCodeStrings[] = {
    [BOOT_INFORMATION_MISSING]          = "BOOT_INFORMATION_MISSING",
    [MEMORY_NO_SPACE]                   = "MEMORY_NO_SPACE",
    [MEMORY_MAP_CORRUPTED]              = "MEMORY_MAP_CORRUPTED",
    [OUT_OF_MEMORY]                     = "OUT_OF_MEMORY",
};

PCHAR MintBugCodeMessages[] = {
    [BOOT_INFORMATION_MISSING]          = "Boot information missing or corrupted.",
    [MEMORY_NO_SPACE]                   = "Not enough space for memory management objects.",
    [MEMORY_MAP_CORRUPTED]              = "The physical memory map has been corrupted",
    [OUT_OF_MEMORY]                     = "Not enough memory is available to load MiNT.",
};



VOID MintBugCheckWithMessage(UINT32 Bugcode, PCSTR Format, ...) {
    UiSetColors(GFX_COLOR_WHITE, GFX_COLOR_BLUE);
    UiClearScreen();

    UiPrint("MINTLDR v1.0 encountered an error while loading.\n\n");
    UiPrint("STOP (0x%8x): %s\n", Bugcode, MintBugCodeStrings[Bugcode]);
    UiPrint("%s\n\n", MintBugCodeMessages[Bugcode]);

    if (Format) {
        UiPrint("Additional information:\n");

        va_list ap;
        va_start(ap, Format);
        UiPrintVA((PCHAR)Format, ap);
        va_end(ap);

        UiPrint("\n");
    }

    UiPrint("Report this error on the MiNT GitHub repository.\n");

    for (;;);
}

VOID MintBugCheck(UINT32 Bugcode) {
    MintBugCheckWithMessage(Bugcode, NULL);
}