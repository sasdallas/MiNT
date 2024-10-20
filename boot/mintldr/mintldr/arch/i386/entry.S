// =======================================================================================================
// FILE:         entry.S
// DESCRIPTION:  i386 entrypoint
// AUTHOR:       Samuel S.
// CREATED:      October 13th, 2024
// LICENSE:      Under the license of MiNT (BSD 3-clause)
// =======================================================================================================

#include <asm.inc>
#include <reactos/x86common.h>

.code32

.extern _IdtInit
.extern _i386Idt
.extern _MintStart

.global _arch_entrypoint
_arch_entrypoint:
    cli

    /* Setup segments */
    mov ax, PMODE_DS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    /* Setup stack */
    mov esp, dword ptr ds:[stack32] // x86common says to do this

    /* Load IDT */
    lidt i386_IDTDescriptor


/* Continue address. This is modified for BIOS32 calls */
.global _arch_continue
_arch_continue:
    .long _arch_startup


/* The main entrypoint for the first time */
_arch_startup:

    /* Load the IDT */
    call _IdtInit
    sti

    call _MintStart

    cli
    hlt


/* Switching to real mode function */
/* SwitchToRealMode_Address is set by _arch_entrypoint to the BSS data of the address */
/* This BSS data is in turn set by realmode startup */
SwitchToRealMode:
    /* pmode segments */
    mov ax, PMODE_DS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    /* Save stack pointer */
    mov dword ptr ds:[stack32], esp

    /* Encode far jump */
    .byte HEX(0ea)
SwitchToRealMode_Address:
    .long 0
    .word RMODE_CS
    nop
    

.global i386_IDTDescriptor
i386_IDTDescriptor:
    .word 0xFFFF
    .long _i386Idt

stack16:
    .word HEX(6F00)

stack32:
    .long HEX(F000)