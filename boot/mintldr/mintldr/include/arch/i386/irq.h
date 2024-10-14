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

// IRQ definitions
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

#endif