/**
 * @file mintldr/arch/i386/idt.c
 * @brief I386 interrupt descriptor table handler
 * 
 * This file handles initializing the IDT. entry.S will call into it and it will call idt.S to initialize the IDT table
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <mintldr.h>
#include <arch/i386/irq.h>
#include <ndk/arch/i386/ke.h>


KI386_IDT_ENTRY DECLSPEC_ALIGN(4) i386Idt[32]; // IDT entries
KDESCRIPTOR i386IdtDescriptor; // IDT descriptor

extern VOID InstallIdt(PKDESCRIPTOR i386IdtDescriptor);


static
void
IdtSetVector(
    _In_ UCHAR Vector,
    _In_ PVOID ServiceHandler, 
    _In_ USHORT Access,
    _In_ USHORT Segment
) {
    if (Vector > 32)
    {
        // TODO: Fail
    }

    i386Idt[Vector].BaseLow = (USHORT)ServiceHandler & 0xFFFF;
    i386Idt[Vector].BaseHi = (USHORT)ServiceHandler >> 16;
    i386Idt[Vector].Segment = Segment;
    i386Idt[Vector].Access = Access;
}

/* Initialize the IDT */
void 
__cdecl 
IdtInit() {
    /* Initialize IDT descriptor */
    i386IdtDescriptor.Limit = 255;
    i386IdtDescriptor.Base = (ULONG)&i386Idt;

    /* Initialize handlers */
    IdtSetVector(0, i386DivByZero, 0x8E00, 0x08);
    IdtSetVector(1, i386DebugException, 0x8E00, 0x08);
    IdtSetVector(2, i386NMIException, 0x8E00, 0x08);
    IdtSetVector(3, i386Breakpoint, 0x8E00, 0x08);
    IdtSetVector(4, i386Overflow, 0x8E00, 0x08);
    IdtSetVector(5, i386BoundException, 0x8E00, 0x08);
    IdtSetVector(6, i386InvalidOpcodeException, 0x8E00, 0x08);
    IdtSetVector(7, i386FPUNotAvailableException, 0x8E00, 0x08);
    IdtSetVector(8, i386DoubleFault, 0x8E00, 0x08);
    IdtSetVector(9, i386CoprocessorSegmentException, 0x8E00, 0x08);
    IdtSetVector(10, i386InvaildTSS, 0x8E00, 0x08);
    IdtSetVector(11, i386SegmentNotPresent, 0x8E00, 0x08);
    IdtSetVector(12, i386StackException, 0x8E00, 0x08);
    IdtSetVector(13, i386GeneralProtectionFault, 0x8E00, 0x08);
    IdtSetVector(14, i386PageFault, 0x8E00, 0x08);


    /* Install IDT */
    //InstallIdt(&i386IdtDescriptor);
}



