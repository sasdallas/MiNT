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
    "*** CPU Exception 00: DIVIDE BY ZERO",
    "*** CPU Exception 01: DEBUG EXCEPTION",
    "*** CPU Exception 02: NON-MASKABLE INTERRUPT EXCEPTION",
    "*** CPU Exception 03: DEBUGGER BREAKPOINT",
    "*** CPU Exception 04: OVERFLOW",
    "*** CPU Exception 05: BOUND EXCEPTION",
    "*** CPU Exception 06: INVALID OPCODE",
    "*** CPU Exception 07: FPU NOT AVAILABLE",
    "*** CPU Exception 08: DOUBLE FAULT",
    "*** CPU Exception 09: COPROCESSOR SEGMENT OVERRUN",
    "*** CPU Exception 10: INVALID TSS",
    "*** CPU Exception 11: SEGMENT NOT PRESENT",
    "*** CPU Exception 12: STACK EXCEPTION",
    "*** CPU Exception 13: GENERAL PROTECTION FAULT",
    "*** CPU Exception 14: PAGE FAULT",
    "*** CPU Exception 15: Reserved",
    "*** CPU Exception 16: COPROCESSOR ERROR",
    "*** CPU Exception 17: ALIGNMENT CHECK",
    "*** CPU Exception 18: MACHINE CHECK"
};

/* Handler method */
void
__cdecl
i386ExceptionHandler(ULONG TrapIndex, PKTRAP_FRAME TrapFrame) {
    // GfxDrawPopup(0, 0, VGA_WIDTH, VGA_HEIGHT, "MiNT - System Exception", "Fatal exception occurred");

    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiClearScreen();
    UiPrint("MINTLDR encountered a fatal exception while loading.\n");
    UiPrint("%s", i386ExceptionMessages[TrapIndex]);

    for (;;);

}