/**
 * @file boot/arch/x86_64/main.c
 * @brief MiNT loader main
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <winnt.h>
#include <ntdef.h>
#include <ui.h>

void __stdcall MintLoaderMain() {
    UiPrint("MINTLDR v1\nHello, MiNT world\n\n");

    UiPrint("Character: %c\n", 'c');
    UiPrint("String: %s\n", "str!");
    UiPrint("Decimal: %i\n", 42);
    UiPrint("Hexadecimal: 0x%x\n", 0xDEADBEEF);

    UiPrint("Padded decimal: %16i\n", 1234);
    UiPrint("Padded hex: 0x%16x", 0xDEADBEEF);

    for (;;);
}