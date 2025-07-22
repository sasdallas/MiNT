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
#include <dbg.h>

#include <arch/serial.h>

void __stdcall MintLoaderMain(UINT32 MultibootHeader, UINT32 MultibootMagic) {
    UiPrint("MINTLDR v1\nHello, MiNT world\n\n");

    UiPrint("Multiboot header: 0x%x\n", MultibootHeader);
    UiPrint("Multiboot magic: 0x%x\n", MultibootMagic);

    /* Initialize the serial port */
    SerialInitialize(0, 38400);

    /* Say hello */
    INFO("MINTLDR v1.0\n");

    for (;;);
}