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




/* Descriptors (IDT/GDT/...) */
typedef struct _KDESCRIPTOR {
    USHORT Limit;
    ULONG Base;
} KDESCRIPTOR, *PKDESCRIPTOR;

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
} KTRAP_FRAME, *PKTRAP_FRAME;