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
i386ExceptionHandler(ULONG TrapIndex, PKTRAP_FRAME TrapFrame, PKSPECIAL_REGISTERS SpecialFrame) {
    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiClearScreen(); 

    GfxDrawPopup(0, 0, VGA_WIDTH, VGA_HEIGHT, "MINTLDR - Fatal Error", "");
    
    // GfxDrawPopup will be replaced by UiCreatePopup eventually, but for now write over the text
    UiSetColors(COLOR_BLACK, COLOR_LIGHT_GRAY);
    UiSetPosition(0, 0);
    UiPrint("MINTLDR v%s - Fatal Error", VERSION_STR);

    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiSetPosition(0, 2);
    

    UiPrint("MINTLDR encountered a fatal exception while loading.\n");
    UiPrint("%s\n\n", i386ExceptionMessages[TrapIndex]);

#ifdef _M_IX86
    UiPrint("Registers:\n");
    UiPrint("EAX: %8x\tESP: %8x\tCR0: %8x\tDR0: %8x\n",
            TrapFrame->Eax, TrapFrame->HardwareEsp, SpecialFrame->Cr0, TrapFrame->Dr0);
    UiPrint("EBX: %8x\tEBP: %8x\tCR1: _UNKNOWN\tDR1: %8x\n",
            TrapFrame->Ebx, TrapFrame->Ebp, TrapFrame->Dr1);
    UiPrint("ECX: %8x\tESI: %8x\tCR2: %8x\tDR2: %8x\n", 
            TrapFrame->Ecx, TrapFrame->Esi, SpecialFrame->Cr2, TrapFrame->Dr2);
    UiPrint("EDX: %8x\tEDI: %8x\tCR3: %8x\tDR3: %8x\n",
            TrapFrame->Edx, TrapFrame->Edi, SpecialFrame->Cr3, TrapFrame->Dr3);
    UiPrint("CR4: %8x\tDR6: %8x\tDR7: %8x\n",
            SpecialFrame->Cr4, TrapFrame->Dr6, TrapFrame->Dr7);
    

    UiPrint("GDTR Base: %8x\tGDTR Limit: %8x\n",
                SpecialFrame->Gdtr.Base, SpecialFrame->Gdtr.Limit);
    UiPrint("IDTR Base: %8x\tIDTR Limit: %8x\n",
                SpecialFrame->Idtr.Base, SpecialFrame->Idtr.Limit);

    UiPrint("\nException Information:\n");

    UiPrint("EIP: %8x\tERR: %8x\tEFL: %8x\n",
                TrapFrame->Eip, TrapFrame->ErrCode, TrapFrame->EFlags);

    UiPrint("\nSegments:\n");
    UiPrint("CS: %4x\tDS: %4x\tES: %4x\tFS: %4x\n",
                (USHORT)TrapFrame->SegCs, (USHORT)TrapFrame->SegDs, (USHORT)TrapFrame->SegEs, (USHORT)TrapFrame->SegFs);
    UiPrint("SS: %4x\n",
                (USHORT)TrapFrame->HardwareSegSs);
        

    /*UiPrint("LDTR: %4x\tTR: %4x\n",
                SpecialFrame->Ldtr, SpecialFrame->Tr); */

#else
    UiPrint("Unknown/in development architecture\n");
#endif

    UiPrint("\nReport this error to the MiNT GitHub repository.\n");

    for (;;);

}