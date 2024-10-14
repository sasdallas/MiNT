/**
 * @file mintldr/arch/i386/irq.c
 * @brief IRQ handler for I386 platform
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
#include <ndk/arch/i386/ke.h>

static const CHAR *i386ExceptionMessages[] =
{
    "Exception 00: DIVIDE BY ZERO",
    "Exception 01: DEBUG EXCEPTION",
    "Exception 02: NON-MASKABLE INTERRUPT EXCEPTION",
    "Exception 03: BREAKPOINT (INT 3)",
    "Exception 04: OVERFLOW",
    "Exception 05: BOUND EXCEPTION",
    "Exception 06: INVALID OPCODE",
    "Exception 07: FPU NOT AVAILABLE",
    "Exception 08: DOUBLE FAULT",
    "Exception 09: COPROCESSOR SEGMENT OVERRUN",
    "Exception 0A: INVALID TSS",
    "Exception 0B: SEGMENT NOT PRESENT",
    "Exception 0C: STACK EXCEPTION",
    "Exception 0D: GENERAL PROTECTION FAULT",
    "Exception 0E: PAGE FAULT",
    "Exception 0F: Reserved",
    "Exception 10: COPROCESSOR ERROR",
    "Exception 11: ALIGNMENT CHECK",
    "Exception 12: MACHINE CHECK"
};

/* Handler method */
void
__cdecl
i386ExceptionHandler(ULONG TrapIndex, PKTRAP_FRAME TrapFrame) {
    // GfxDrawPopup(0, 0, VGA_WIDTH, VGA_HEIGHT, "MiNT - System Exception", "Fatal exception occurred");

    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiClearScreen();
    UiPrint("Fatal exception occurred\n");
    UiPrint("%s", i386ExceptionMessages[TrapIndex]);

    for (;;);

}