/**
 * @file ndk/arch/i386/ke.h
 * @brief Kernel types for I386 architecture
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */


#pragma once

#include <xdk/ketypes.h>

/* Descriptors (IDT/GDT/...) */
typedef struct _KDESCRIPTOR {
    USHORT Limit;
    ULONG Base;
} __attribute__((packed)) KDESCRIPTOR, *PKDESCRIPTOR;

/* IDT entry */
typedef struct _KI386_IDT_ENTRY {
    USHORT BaseLow;
    USHORT Segment;
    USHORT Access;
    USHORT BaseHi;
} __attribute__((packed)) KI386_IDT_ENTRY, *PKI386_IDT_ENTRY;

/* Trap Frame */
typedef struct _KTRAP_FRAME {
    ULONG DbgEbp;
    ULONG DbgEip;
    ULONG ArgMark;
    ULONG ArgPointer;
    ULONG TempSegCs;
    ULONG TempEsp;
    ULONG Dr0;
    ULONG Dr1;
    ULONG Dr2;
    ULONG Dr3;
    ULONG Dr6;
    ULONG Dr7;
    ULONG SegGs;
    ULONG SegEs;
    ULONG SegDs;
    ULONG Edx;
    ULONG Ecx;
    ULONG Eax;
    ULONG PreviousPreviousMode; // UNUSED
    VOID *ExceptionList; // UNUSED
    ULONG SegFs;
    ULONG Edi;
    ULONG Esi;
    ULONG Ebx;
    ULONG Ebp;

    /* Pushed by CPU */

    ULONG ErrCode;
    ULONG Eip;
    ULONG SegCs;
    ULONG EFlags;
    ULONG HardwareEsp;
    ULONG HardwareSegSs;
    ULONG V86Es;
    ULONG V86Ds;
    ULONG V86Fs;
    ULONG V86Gs;
} __attribute__((packed)) KTRAP_FRAME, *PKTRAP_FRAME;

typedef struct _KSPECIAL_REGISTERS {
    ULONG Cr0;
    ULONG Cr2;
    ULONG Cr3;
    ULONG Cr4;
    ULONG KernelDr0;
    ULONG KernelDr1;
    ULONG KernelDr2;
    ULONG KernelDr3;
    ULONG KernelDr6;
    ULONG KernelDr7;
    KDESCRIPTOR Gdtr;
    KDESCRIPTOR Idtr;
    USHORT Tr;
    USHORT Ldtr;
    ULONG Reserved[6];
} KSPECIAL_REGISTERS, *PKSPECIAL_REGISTERS;