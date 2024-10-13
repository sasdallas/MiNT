/**
 * @file mintldr/arch/i386/idt.S
 * @brief IDT installer
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <asm.inc>

.global _InstallIdt
_InstallIdt:
    mov eax, [esp + 4]
    lidt [eax] 
    ret

.extern _i386ExceptionHandler@12:PROC

i386CommonExceptionHandler:
    /* Push KTRAP_FRAME */
    push ebp
    push ebx
    push esi
    push edi
    push fs
    push 0
    push 0
    push eax
    push ecx
    push edx
    push ds
    push es
    push gs
    mov eax, dr7
    push eax
    mov eax, dr3
    push eax
    mov eax, dr2
    push eax
    mov eax, dr1
    push eax
    mov eax, dr0
    push eax
    sub esp, 6 * 4
    
    push dword ptr ds:[i386ExceptionIndex]

    call _i386ExceptionHandler

    cli
    hlt



.macro IRQ name index
    .global name
    &name:
        push 0
        mov dword ptr ds:[i386ExceptionIndex], VAL(index)
        jmp i386CommonExceptionHandler
.endm


.macro ISR_NOERRCODE name index
    .global VAL(name)
    &name:
        push 0
        mov dword ptr ds:[i386ExceptionIndex], VAL(index)
        jmp i386CommonExceptionHandler
.endm

.macro ISR_ERRCODE name index
    .global VAL(name)
    &name:
        mov dword ptr ds:[i386ExceptionIndex], VAL(index)
        jmp i386CommonExceptionHandler
.endm

/* Exceptions */
ISR_NOERRCODE   _i386DivByZero, 0
ISR_NOERRCODE   _i386DebugException, 1
ISR_NOERRCODE   _i386NMIException, 2
ISR_NOERRCODE   _i386Breakpoint, 3
ISR_NOERRCODE   _i386Overflow, 4
ISR_NOERRCODE   _i386BoundException, 5
ISR_NOERRCODE   _i386InvalidOpcodeException, 6
ISR_NOERRCODE   _i386FPUNotAvailableException, 7
ISR_ERRCODE     _i386DoubleFault, 8
ISR_NOERRCODE   _i386CoprocessorSegmentException, 9
ISR_ERRCODE     _i386InvaildTSS, 10
ISR_ERRCODE     _i386SegmentNotPresent, 11
ISR_ERRCODE     _i386StackException, 12
ISR_ERRCODE     _i386GeneralProtectionFault, 13
ISR_ERRCODE     _i386PageFault, 14
// (15 reserved)
ISR_NOERRCODE   _i386CoprocessorError, 16
ISR_NOERRCODE   _i386AlignmentCheck, 17
ISR_NOERRCODE   _i386MachineCheck, 18
ISR_NOERRCODE   _i386SIMDFloatError, 19

/* Sending KTRAP_FRAME is annoying because it clutters up the stack so this is a better way */
i386ExceptionIndex:
    .long 0
