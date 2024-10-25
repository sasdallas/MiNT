/**
 * @file mintldr/include/arch/i386/irq.h
 * @brief IRQ definitions
 * 
 * 
 * @copyright
 * This file is part of MiNT which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */


#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

/* Exceptions */
void __cdecl i386DivByZero(void);
void __cdecl i386DebugException(void);
void __cdecl i386NMIException(void);
void __cdecl i386Breakpoint(void);
void __cdecl i386Overflow(void);
void __cdecl i386BoundException(void);
void __cdecl i386InvalidOpcodeException(void);
void __cdecl i386FPUNotAvailableException(void);
void __cdecl i386DoubleFault(void);
void __cdecl i386CoprocessorSegmentException(void);
void __cdecl i386InvaildTSS(void);
void __cdecl i386SegmentNotPresent(void);
void __cdecl i386StackException(void); 
void __cdecl i386GeneralProtectionFault(void); 
void __cdecl i386PageFault(void); 
void __cdecl i386VirtualizationException(void);

void __cdecl i386CoprocessorError(void);
void __cdecl i386AlignmentCheck(void);
void __cdecl i386MachineCheck(void);
void __cdecl i386SIMDFloatError(void);
void __cdecl i386VirtualizationException(void);


/* Reserved */
void __cdecl i386Reserved0(void);
void __cdecl i386Reserved1(void);
void __cdecl i386Reserved2(void);
void __cdecl i386Reserved3(void);
void __cdecl i386Reserved4(void);
void __cdecl i386Reserved5(void);
void __cdecl i386Reserved6(void);
void __cdecl i386Reserved7(void);
void __cdecl i386Reserved8(void);
void __cdecl i386Reserved9(void);
void __cdecl i386Reserved10(void);
void __cdecl i386Reserved11(void);

/* IRQs */
void __cdecl i386IRQ0(void); 
void __cdecl i386IRQ1(void);
void __cdecl i386IRQ2(void); 
void __cdecl i386IRQ3(void); 
void __cdecl i386IRQ4(void); 
void __cdecl i386IRQ5(void);    
void __cdecl i386IRQ6(void);
void __cdecl i386IRQ7(void); 
void __cdecl i386IRQ8(void); 
void __cdecl i386IRQ9(void); 
void __cdecl i386IRQ10(void);
void __cdecl i386IRQ11(void);
void __cdecl i386IRQ12(void);
void __cdecl i386IRQ13(void);
void __cdecl i386IRQ14(void);
void __cdecl i386IRQ15(void);


#endif