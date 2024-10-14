/**
 * @file mintldr/mintldr.c
 * @brief Main MINTLDR file
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
#include <ui/ui.h>

void
__cdecl
MintStart() {
    UiInit();
    UiPrint("MINTLDR v%s\n", VERSION_STR);
    UiPrint("Starting MiNT\n");

    UiPrint("\nUiPrint Testing\n");
    UiPrint("Integer: %i\n", 5);
    UiPrint("String: %s\n", "HELLO");
    UiPrint("Character: %c\n", 'g');
    UiPrint("Hexadecimal: 0x%x\n", 0xDEADBEEF);
    UiPrint("\tTabbing\n");

    int thing = 0;
    int test = 5 / thing;
}
