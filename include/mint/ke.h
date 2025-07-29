/**
 * @file include/mint/ke.h
 * @brief Kernel header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_KE_H__
#define __MINT_KE_H__ 1

__MINT_BEGIN_DECLS

#include <ketypes.h>
#include <winnt.h>
#include <hardware/hardware.h>
#include <bugcodes.h>

/* Initial IDT entry */
typedef struct _KIDT_INIT_ENTRY {
    UCHAR Vector;               // IDT vector assigned
    UCHAR Dpl;                  // DPL
    UCHAR IstIndex;             // IST index
    PVOID ServiceRoutine;       // Service routine
} KIDT_INIT_ENTRY, *PKIDT_INIT_ENTRY;

typedef struct _KIDTENTRY64 {
    USHORT OffsetLow;           // Low bits of ffset
    USHORT Selector;            // Segment selector
    USHORT IstIndex:3;          // IST index
    USHORT Reserved0:5;         // Reserved
    USHORT Type:5;              // Type
    USHORT Dpl:2;               // DPL
    USHORT Present:1;           // Present
    USHORT OffsetMiddle;        // Middle offset
    ULONG OffsetHigh;           // Offset high
    ULONG Reserved1;            // Reserved
} KIDTENTRY64, *PKIDTENTRY64;

/* Exception handlers */
VOID KiDivisionByZeroException();
VOID KiDebugTrapOrFault();
VOID KiNmiInterrupt();

NTAPI
VOID
KeInitExceptions();

NTAPI
VOID
KeBugCheckWithFrame(
    ULONG BugCheckCode,
    ULONG_PTR BugCheckParameter1,
    ULONG_PTR BugCheckParameter2,
    ULONG_PTR BugCheckParameter3,
    ULONG_PTR BugCheckParameter4,
    PKTRAP_FRAME TrapFrame
);

NTAPI
VOID
KeBugCheckEx(
    ULONG BugCheckCode,
    ULONG_PTR BugCheckParameter1,
    ULONG_PTR BugCheckParameter2,
    ULONG_PTR BugCheckParameter3,
    ULONG_PTR BugCheckParameter4
);

NTAPI
VOID
KeBugCheck(
    ULONG BugCheckCode
);

VOID
KiDisableInterrupts();

VOID 
KiEnableInterrupts();

__MINT_END_DECLS

#endif