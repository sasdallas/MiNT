/**
 * @file mintkrnl/ke/x86_64/except.c
 * @brief x86_64 exception handler
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <ke.h>
#include <kddll.h>

KIDT_INIT_ENTRY KiInitialIdtEntries[] = { 
    { .Vector = 0x00, .Dpl = 0x00, .IstIndex = 0x00, .ServiceRoutine = KiDivisionByZeroException }
};

KIDTENTRY64 KiIdt[256];
KDESCRIPTOR KiIdtr = { .Pad = { 0 }, .Limit = sizeof(KiIdt) - 1, .Base = KiIdt };

STATIC
VOID
KeInstallExceptionVector(UCHAR Vector, UCHAR Dpl, UCHAR IstIndex, PVOID ServiceRoutine) {
    KiIdt[Vector].Type = 0x0E;
    KiIdt[Vector].OffsetLow = (ULONG64)ServiceRoutine & 0xFFFF;
    KiIdt[Vector].OffsetMiddle = ((ULONG64)ServiceRoutine >> 16) & 0xFFFF;
    KiIdt[Vector].OffsetHigh = ((ULONG64)ServiceRoutine >> 32);
    KiIdt[Vector].Reserved0 = 0;
    KiIdt[Vector].Reserved1 = 0; 
    KiIdt[Vector].Present = 1;
    KiIdt[Vector].Selector = 0x08;
    KiIdt[Vector].Dpl = Dpl;
    KiIdt[Vector].IstIndex = IstIndex;
}

VOID
__attribute__((naked))
KiUnexpectedInterrupt()
{
    /* TODO : Bugcheck */
    for (;;);
}


NTAPI
VOID
KeInitExceptions() 
{
    /* Build the IDT */
    for (int i = 0, j = 0; i < 256; i++) {
        /* Does the initial IDT descriptor list have an entry for this? */
        if (KiInitialIdtEntries[j].Vector == i) {
            KeInstallExceptionVector(i, KiInitialIdtEntries[j].Dpl, KiInitialIdtEntries[j].IstIndex, KiInitialIdtEntries[j].ServiceRoutine);
        } else {
            /* TODO: Should have a list of KiUnexpectedInterrupt handlers that push vector number */
            KeInstallExceptionVector(i, 0, 0, KiUnexpectedInterrupt);
        }
    }

    /* IDT built, install. */
    __asm__ __volatile__("lidt %0" : : "m"(*(short*)(&KiIdtr.Limit)));

    int a = 45;
    int b = a / 0;
    KdpDebugPrint("Result: %d\n", b);
}


VOID
KiDisableInterrupts() {
    __asm__ __volatile__ ("cli");
}

VOID 
KiEnableInterrupts() {
    __asm__ __volatile__ ("sti");
}