/**
 * @file mintldr/include/arch/i386/i386.h
 * @brief I386 main header file definitions
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef ARCH_I386_H
#define ARCH_I386_H

/* EFLAGS masks */
#ifdef __ASM__
#define EFLAGS_CF HEX(01)
#define EFLAGS_ZF HEX(40)
#define EFLAGS_SF HEX(80)
#endif

#ifndef __ASM__

/** Union registers **/

/* DWORD registers */
typedef struct
{
    unsigned long    eax;
    unsigned long    ebx;
    unsigned long    ecx;
    unsigned long    edx;
    unsigned long    esi;
    unsigned long    edi;
    unsigned long    ebp;

    unsigned short    ds;
    unsigned short    es;
    unsigned short    fs;
    unsigned short    gs;

    unsigned long    eflags;

} REGS_DWORD;

/* WORD registers */
typedef struct
{
    unsigned short    ax, _upper_ax;
    unsigned short    bx, _upper_bx;
    unsigned short    cx, _upper_cx;
    unsigned short    dx, _upper_dx;
    unsigned short    si, _upper_si;
    unsigned short    di, _upper_di;
    unsigned short    bp, _upper_bp;

    unsigned short    ds;
    unsigned short    es;
    unsigned short    fs;
    unsigned short    gs;

    unsigned short    flags, _upper_flags;
} REGS_WORD;

/* BYTE registers */ 
typedef struct
{
    unsigned char    al;
    unsigned char    ah;
    unsigned short    _upper_ax;
    unsigned char    bl;
    unsigned char    bh;
    unsigned short    _upper_bx;
    unsigned char    cl;
    unsigned char    ch;
    unsigned short    _upper_cx;
    unsigned char    dl;
    unsigned char    dh;
    unsigned short    _upper_dx;
    unsigned short    si, _upper_si;
    unsigned short    di, _upper_di;
    unsigned short    bp, _upper_bp;

    unsigned short    ds;
    unsigned short    es;
    unsigned short    fs;
    unsigned short    gs;

    unsigned short    flags, _upper_flags;
} REGS_BYTE;


/* All of these equivocate to the same size */
/* Source: ReactOS */
typedef union
{
    REGS_DWORD       x;
    REGS_DWORD       d;
    REGS_WORD        w;
    REGS_BYTE        b;
} REGISTERS;


/* BIOS32 call */
int __cdecl bios32(int vector, REGISTERS* input, REGISTERS* output);

/* CF is set call usually failed */
#define BIOS32_SUCCESS(regs) ((regs.x.eflags & 0x01) == 0)



#endif 

/* Structure layout */
#define REGS_EAX 0
#define REGS_EBX 4
#define REGS_ECX 8
#define REGS_EDX 12
#define REGS_ESI 16
#define REGS_EDI 20
#define REGS_EBP 24
#define REGS_DS 28
#define REGS_ES 30
#define REGS_FS 32
#define REGS_GS 34
#define REGS_EFLAGS 36
#define REGS_SIZE 40



/* BIOS32 buffer parameters */
#define BIOSCALLBUFFER      0x4000 /* Temporary BIOS call buffer */
#define BIOSCALLBUFSEGMENT (BIOSCALLBUFFER/16) /* Buffer to store temporary data for any BIOS32 call */
#define BIOSCALLBUFOFFSET   0x0000 /* Buffer to store temporary data for any BIOS32 call */
#define BIOSCALLBUFSIZE     4096  


#endif