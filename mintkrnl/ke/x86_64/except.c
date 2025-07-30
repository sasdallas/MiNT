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
    { .Vector = 0x00, .Dpl = 0x00, .IstIndex = 0x00, .ServiceRoutine = KiDivisionByZeroException },
    { .Vector = 0x0E, .Dpl = 0x00, .IstIndex = 0x00, .ServiceRoutine = KiPageFaultException },
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
    KeBugCheck(KMODE_EXCEPTION_NOT_HANDLED);
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
            j++;
        } else {
            /* TODO: Should have a list of KiUnexpectedInterrupt handlers that push vector number */
            KeInstallExceptionVector(i, 0, 0, KiUnexpectedInterrupt);
        }
    }

    /* IDT built, install. */
    __asm__ __volatile__("lidt %0" : : "m"(*(short*)(&KiIdtr.Limit)));

    *(PUINT32)0xDEADBEEF = 12345;
}


VOID
KiDisableInterrupts() {
    __asm__ __volatile__ ("cli");
}

VOID 
KiEnableInterrupts() {
    __asm__ __volatile__ ("sti");
}

NTAPI
VOID
KePrintTrapFrame(
    PKTRAP_FRAME TrapFrame
)
{
    KdpDebugPrint("RAX %016llX RBX %016llX RCX %016llX RDX %016llX\n", TrapFrame->Rax, TrapFrame->Rbx, TrapFrame->Rcx, TrapFrame->Rdx);
    KdpDebugPrint("RDI %016llX RSI %016llX R8  %016llX R9  %016llX\n", TrapFrame->Rdi, TrapFrame->Rsi, TrapFrame->R8, TrapFrame->R9);
    KdpDebugPrint("R10 %016llX R11 %016llX ERR %016llX\n", TrapFrame->R10, TrapFrame->R11, TrapFrame->ErrorCode);
    KdpDebugPrint("DS %04x ES %04x FS %04x GS %04x SS %04x\n\n", TrapFrame->SegDs, TrapFrame->SegEs, TrapFrame->SegFs, TrapFrame->SegGs, TrapFrame->SegSs);

    KdpDebugPrint("EXCEPTION OCCURRED AT %04x:%016llX\n", TrapFrame->SegCs, TrapFrame->Rip);
    KdpDebugPrint("STACK: %016llX BP: %016llX\n", TrapFrame->Rsp, TrapFrame->Rbp);
}