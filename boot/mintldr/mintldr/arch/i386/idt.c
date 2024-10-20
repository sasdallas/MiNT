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
#include <arch/i386/mach.h>
#include <arch/i386/irq.h>
#include <ndk/arch/i386/ke.h>


KI386_IDT_ENTRY i386Idt[256]; // IDT entries
EXTERN PKDESCRIPTOR _i386_IDTDescriptor; // IDT descriptor

extern VOID InstallIdt(PKDESCRIPTOR i386IdtDescriptor);


static
void
IdtSetVector(
    _In_ UCHAR Vector,
    _In_ PVOID ServiceHandler, 
    _In_ USHORT Access,
    _In_ USHORT Segment
) {
    i386Idt[Vector].BaseLow = (ULONG)ServiceHandler & 0xFFFF;
    i386Idt[Vector].BaseHi = (ULONG)ServiceHandler >> 16;
    i386Idt[Vector].Segment = Segment;
    i386Idt[Vector].Access = Access;
}


/* Send PIC command */
STATIC
VOID
IdtSendPicCommand(
    _In_ UINT PicAddress,
    _In_ UINT PicOffset,
    _In_ UCHAR Data
)
{
    WRITE_PORT_UCHAR(PicAddress + PicOffset, Data);
}

/* Read PIC */
STATIC
UCHAR
IdtReadPic(
    _In_ UCHAR PicAddress
)
{
    return READ_PORT_UCHAR(PicAddress + MACH_I386_PIC_DATA);
}

/* Handle PIC end-of-interrupt (imported by EXTERN) */
VOID
IdtSendEoi(
    _In_ UCHAR InterruptNumber
)
{
    /* Interrupts above 8 need to be remapped in slave PIC. */
    if (InterruptNumber >= 8)
    {
        IdtSendPicCommand(MACH_I386_PIC2_IO, MACH_I386_PIC_CMD, 0x20);
    }

    /* 0x20 is EOI */
    IdtSendPicCommand(MACH_I386_PIC1_IO, MACH_I386_PIC_CMD, 0x20);
}




/* Initialize the IDT */
void 
__cdecl 
IdtInit()
{

    /* Initialize handlers */
    IdtSetVector(0, &i386DivByZero, 0x8E00, 0x08);
    IdtSetVector(1, &i386DebugException, 0x8E00, 0x08);
    IdtSetVector(2, &i386NMIException, 0x8E00, 0x08);
    IdtSetVector(3, &i386Breakpoint, 0x8E00, 0x08);
    IdtSetVector(4, &i386Overflow, 0x8E00, 0x08);
    IdtSetVector(5, &i386BoundException, 0x8E00, 0x08);
    IdtSetVector(6, &i386InvalidOpcodeException, 0x8E00, 0x08);
    IdtSetVector(7, &i386FPUNotAvailableException, 0x8E00, 0x08);
    IdtSetVector(8, &i386DoubleFault, 0x8E00, 0x08);
    IdtSetVector(9, &i386CoprocessorSegmentException, 0x8E00, 0x08);
    IdtSetVector(10, &i386InvaildTSS, 0x8E00, 0x08);
    IdtSetVector(11, &i386SegmentNotPresent, 0x8E00, 0x08);
    IdtSetVector(12, &i386StackException, 0x8E00, 0x08);
    IdtSetVector(13, &i386GeneralProtectionFault, 0x8E00, 0x08);
    IdtSetVector(14, &i386PageFault, 0x8E00, 0x08);
    IdtSetVector(15, &i386Reserved0, 0x8E00, 0x08);
    IdtSetVector(16, &i386CoprocessorError, 0x8E00, 0x08);
    IdtSetVector(17, &i386AlignmentCheck, 0x8E00, 0x08);
    IdtSetVector(18, &i386MachineCheck, 0x8E00, 0x08);
    IdtSetVector(19, &i386SIMDFloatError, 0x8E00, 0x08);
    IdtSetVector(20, &i386VirtualizationException, 0x8E00, 0x08);

    IdtSetVector(32, &i386IRQ0, 0x8E00, 0x08);
    IdtSetVector(33, &i386IRQ1, 0x8E00, 0x08);

    /** Reprogram the PIC **/
    /* TODO: This should be constantized and needs to be better */
    /* TODO: This should also implement I/O waits */

    /* Save their masks */
    
    UCHAR PicMask1, PicMask2;
    PicMask1 = IdtReadPic(MACH_I386_PIC1_IO);
    PicMask2 = IdtReadPic(MACH_I386_PIC2_IO);

    /* Begin to initialize */
    IdtSendPicCommand(MACH_I386_PIC1_IO, MACH_I386_PIC_CMD, MACH_I386_ICW1_INIT | MACH_I386_ICW1_ICW4_PRES);
    IdtSendPicCommand(MACH_I386_PIC2_IO, MACH_I386_PIC_CMD, MACH_I386_ICW1_INIT | MACH_I386_ICW1_ICW4_PRES);
    
    /* Write master & slave PIC offset */
    IdtSendPicCommand(MACH_I386_PIC1_IO, MACH_I386_PIC_DATA, 0x20);
    IdtSendPicCommand(MACH_I386_PIC2_IO, MACH_I386_PIC_DATA, 0x28);

    /* Identify slave PIC */
    IdtSendPicCommand(MACH_I386_PIC1_IO, MACH_I386_PIC_DATA, 4);

    /* Give slave PIC cascade identity (00000010b) */
    IdtSendPicCommand(MACH_I386_PIC2_IO, MACH_I386_PIC_DATA, 2);

    /* Put PICs into 8086 mode */
    IdtSendPicCommand(MACH_I386_PIC1_IO, MACH_I386_PIC_DATA, MACH_I386_ICW4_8086MODE);
    IdtSendPicCommand(MACH_I386_PIC2_IO, MACH_I386_PIC_DATA, MACH_I386_ICW4_8086MODE);

    /* Restore the masks */
    IdtSendPicCommand(MACH_I386_PIC1_IO, MACH_I386_PIC_DATA, PicMask1);
    IdtSendPicCommand(MACH_I386_PIC2_IO, MACH_I386_PIC_DATA, PicMask2);


}




