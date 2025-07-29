/**
 * @file mintkrnl/ke/bugcheck.c
 * @brief Kernel bugcheck
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

BOOLEAN
NTAPI 
KeGetBugMessageText(
    IN ULONG BugCheckCode,
    OUT PCHAR *OutputString OPTIONAL) 
{
    /* This is a temporary list */
    /* Supposedly this unmasks the resource area and pulls the string from there */

    /* TODO: English lookup table? */
    switch (BugCheckCode) {
        case KMODE_EXCEPTION_NOT_HANDLED:
            *OutputString = "KMODE_EXCEPTION_NOT_HANDLED";
            return TRUE;
    }

    return FALSE;
}

NTAPI
VOID
KeBugCheckWithFrame(
    ULONG BugCheckCode,
    ULONG_PTR BugCheckParameter1,
    ULONG_PTR BugCheckParameter2,
    ULONG_PTR BugCheckParameter3,
    ULONG_PTR BugCheckParameter4,
    PKTRAP_FRAME TrapFrame
) {
    /* First, disable interrupts */
    KiDisableInterrupts();

    /* TODO: Inbv, this is VERY prototype! */
    KdpDebugPrint("\n*** KERNEL EXCEPTION DETECTED: SYSTEM HALTED\n");
    KdpDebugPrint("*** STOP: 0x%08lx (%p, %p, %p, %p)\n", BugCheckCode, BugCheckParameter1, BugCheckParameter2, BugCheckParameter3, BugCheckParameter4);

    /* Try to get bug message text */
    PCHAR BugMessageText = NULL;
    if (KeGetBugMessageText(BugCheckCode, &BugMessageText)) {
        /* Got bug message text */
        KdpDebugPrint("*** %s\n", BugMessageText);
    }

    KdpDebugPrint("\nFrame: %p\n", TrapFrame);
    if (TrapFrame) {
        KdpDebugPrint("RAX %016llX RBX %016llX RCX %016llX RDX %016llX\n", TrapFrame->Rax, TrapFrame->Rbx, TrapFrame->Rcx, TrapFrame->Rdx);
        KdpDebugPrint("RDI %016llX RSI %016llX R8  %016llX R9  %016llX\n", TrapFrame->Rdi, TrapFrame->Rsi, TrapFrame->R8, TrapFrame->R9);
        KdpDebugPrint("R10 %016llX R11 %016llX\n", TrapFrame->R10, TrapFrame->R11);
        KdpDebugPrint("DS %04x ES %04x FS %04x GS %04x SS %04x\n\n", TrapFrame->SegDs, TrapFrame->SegEs, TrapFrame->SegFs, TrapFrame->SegGs, TrapFrame->SegSs);

        KdpDebugPrint("EXCEPTION OCCURRED AT %04x:%016llX\n", TrapFrame->SegCs, TrapFrame->Rip);
        KdpDebugPrint("STACK: %016llX BP: %016llX\n", TrapFrame->Rsp, TrapFrame->Rbp);
    }

    /* TODO: KiHaltCpu */
    while (1);
}

NTAPI
VOID
KeBugCheckEx(
    ULONG BugCheckCode,
    ULONG_PTR BugCheckParameter1,
    ULONG_PTR BugCheckParameter2,
    ULONG_PTR BugCheckParameter3,
    ULONG_PTR BugCheckParameter4
) {
    KeBugCheckWithFrame(BugCheckCode, BugCheckParameter1, BugCheckParameter2, BugCheckParameter3, BugCheckParameter4, NULL);
}

NTAPI
VOID
KeBugCheck(
    ULONG BugCheckCode
) {
    KeBugCheckEx(BugCheckCode, 0, 0, 0 ,0);
}
