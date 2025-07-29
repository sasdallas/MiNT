/**
 * @file include/mint/hardware/x86_64/x86_64.h
 * @brief Definitions for x86_64 architecture
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_HARDWARE_X86_64_X86_64_H__
#define __MINT_HARDWARE_X86_64_X86_64_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>
#include <ketypes.h>

/* https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/ntos/amd64_x/ktrap_frame.htm */

typedef struct _KTRAP_FRAME {
    ULONG64 P1Home;
    ULONG64 P2Home;
    ULONG64 P3Home;
    ULONG64 P4Home;
    ULONG64 P5;
    
    /* Processor/IRQL */
    KPROCESSOR_MODE PreviousMode;
    KIRQL PreviousIrql;
    UCHAR FaultIndicator;
    UCHAR ExceptionActive;
    ULONG MxCsr;
    
    /* General purpose */
    ULONG64 Rax;
    ULONG64 Rcx;
    ULONG64 Rdx;
    ULONG64 R8;
    ULONG64 R9;
    ULONG64 R10;
    ULONG64 R11;
    
    union {
        ULONG64 GsBase;
        ULONG64 GsSwap;
    };
    
    /* SSE */
    M128A Xmm0;
    M128A Xmm1;
    M128A Xmm2;
    M128A Xmm3;
    M128A Xmm4;
    M128A Xmm5;

    /* Fault addresses */
    union {
        ULONG64 FaultAddress;
        ULONG64 ContextRecord;
        ULONG64 TimeStamp;          /* No longer used in >=1703 */
    };

    /* Debug registers */
    ULONG64 Dr0;
    ULONG64 Dr1;
    ULONG64 Dr2;
    ULONG64 Dr3;
    ULONG64 Dr6;
    ULONG64 Dr7;

    struct {
        ULONG64 DebugControl;
        ULONG64 LastBranchToRip;
        ULONG64 LastBranchFromRip;
        ULONG64 LastExceptionToRip;
        ULONG64 LastExceptionFromRip;
    };


    /* Segments */
    USHORT SegDs;
    USHORT SegEs;
    USHORT SegFs;
    USHORT SegGs;

    /* Trap frame */
    ULONG64 TrapFrame;

    /* General purpose (preserved) */
    ULONG64 Rbx;
    ULONG64 Rdi;
    ULONG64 Rsi;
    ULONG64 Rbp;

    union {
        ULONG64 ErrorCode;
        ULONG64 ExceptionFrame;
    };

    /* CS:RIP */
    ULONG64 Rip;
    USHORT SegCs;

    /* Fill/logging/EFL/etc. */
    UCHAR Fill0;
    UCHAR Logging;
    USHORT Fill1[2];
    ULONG Eflags;
    ULONG Fill2;
    ULONG64 Rsp;
    USHORT SegSs;

    USHORT Fill3;
    ULONG Fill4;
} KTRAP_FRAME, *PKTRAP_FRAME;

__MINT_END_DECLS

#endif