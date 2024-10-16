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
    mov eax, dword ptr [esp + 4]
    lidt [eax]
    ret

.extern _i386ExceptionHandler@12:PROC
.extern _i386InterruptHandler:@12:PROC

.macro SAVE_STATE

    /** Push KTRAP_FRAME **/
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
    mov eax, dr6
    push eax
    mov eax, dr3
    push eax
    mov eax, dr2
    push eax
    mov eax, dr1
    push eax
    mov eax, dr0
    push eax

    /* Skip past Temp/Arg/Dbg sections */
    sub esp, 6 * 4


    /** Push KSPECIAL_REGISTERS **/

    /* Shift stack to right below GDTR */
    sub esp, 44

    sgdt [esp]
    sidt [esp + 8]
    str word ptr [esp + 16]
    sldt word ptr [esp + 18]
    
    mov eax, dr7;
    push eax
    mov eax, dr6;
    push eax
    mov eax, dr3;
    push eax
    mov eax, dr2;
    push eax
    mov eax, dr1;
    push eax
    mov eax, dr0;
    push eax
    mov eax, cr4;
    push eax
    mov eax, cr3;
    push eax
    mov eax, cr2;
    push eax
    mov eax, cr0;
    push eax
.endm

.macro RESTORE_STATE
    /* Skip special registers, those are just for debugging */
    add esp, 21 * 4

    /* Skip temp/arg/debug */
    add esp, 6 * 4

    /* Skip DR registers */
    add esp, 6 * 4

    /* Restore GS (maybe don't clobber ecx?) */
    pop ecx
    mov gs, cx

    /* Restore ES */
    pop ecx
    mov es, cx

    /* Restore DS */
    pop ecx
    mov ds, cx

    /* Restore registers */
    pop edx
    pop ecx
    pop eax

    /* Skip over ExceptionList and PreviousPreviousMode */
    add esp, 8 

    /* Restore FS (trying our best not to clobber) */
    mov fs, [esp]
    add esp, 4

    /* Restore general-purpose registers */
    pop edi
    pop esi
    pop ebx
    pop ebp


.endm

i386CommonExceptionHandler:
    SAVE_STATE

    lea eax, [esp + (21 * 4)]  // KTRAP_FRAME
    push esp // KSPECIAL_REGISTERS
    push eax
    push dword ptr ds:[i386ExceptionIndex]

    call _i386ExceptionHandler

    /* Exceptions do not need to be resolved until custom handler support is impl. */
    cli
    hlt

i386CommonIRQHandler:
    SAVE_STATE
    
    lea eax, [esp + (21 * 4)]
    push esp
    push eax
    push dword ptr ds:[i386IRQIndex]
    push dword ptr ds:[i386ExceptionIndex]

    call _i386InterruptHandler

    /* Adjust stack to be past the last 4 pushes */
    add esp, 16

    /* Restore state */
    RESTORE_STATE

    /* RESTORE_STATE leaves stack right before EIP and CS so we can iret */
    iret

    

.macro IRQ name index
    .global VAL(name)
    &name:
        mov dword ptr ds:[i386IRQIndex], VAL(index) - 32
        mov dword ptr ds:[i386ExceptionIndex], VAL(index)
        jmp i386CommonIRQHandler
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
ISR_NOERRCODE   _i386Reserved0, 15
ISR_NOERRCODE   _i386CoprocessorError, 16
ISR_NOERRCODE   _i386AlignmentCheck, 17
ISR_NOERRCODE   _i386MachineCheck, 18
ISR_NOERRCODE   _i386SIMDFloatError, 19
ISR_NOERRCODE   _i386VirtualizationException, 20

/* Reserved */
ISR_NOERRCODE   _i386Reserved1, 21
ISR_NOERRCODE   _i386Reserved2, 22
ISR_NOERRCODE   _i386Reserved3, 23
ISR_NOERRCODE   _i386Reserved4, 24
ISR_NOERRCODE   _i386Reserved5, 25
ISR_NOERRCODE   _i386Reserved6, 26
ISR_NOERRCODE   _i386Reserved7, 27
ISR_NOERRCODE   _i386Reserved8, 28
ISR_NOERRCODE   _i386Reserved9, 29
ISR_NOERRCODE   _i386Reserved10, 30
ISR_NOERRCODE   _i386Reserved11, 31

/* IRQs */
IRQ             _i386IRQ0, 32
IRQ             _i386IRQ1, 33
IRQ             _i386IRQ2, 34
IRQ             _i386IRQ3, 35
IRQ             _i386IRQ4, 36
IRQ             _i386IRQ5, 37    
IRQ             _i386IRQ6, 38
IRQ             _i386IRQ7, 39
IRQ             _i386IRQ8, 40
IRQ             _i386IRQ9, 41
IRQ             _i386IRQ10, 42
IRQ             _i386IRQ11, 43
IRQ             _i386IRQ12, 44
IRQ             _i386IRQ13, 45
IRQ             _i386IRQ14, 46
IRQ             _i386IRQ15, 47




/* Sending KTRAP_FRAME is annoying because it clutters up the stack so this is a better way */
i386ExceptionIndex:
    .long 0

i386IRQIndex:
    .long 0
