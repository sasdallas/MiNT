// =======================================================================================================
// FILE:         bios32.S
// DESCRIPTION:  BIOS32 systems
// AUTHOR:       Samuel S.
// CREATED:      October 20th, 2024
// LICENSE:      Under the license of MiNT (BSD 3-clause)
// =======================================================================================================

#include <asm.inc>
#include <arch/i386/i386.h>
#include <reactos/x86common.h>

.code32

Bios32RegsInput:
    .long 0
Bios32RegsOutput:
    .long 0


.extern SwitchToRealMode
.extern _arch_continue

//
// bios32(int vector, REGS* input, REGS* output);
//
.global _bios32
_bios32:
    /* Store interrupt vector in BSS data */
    mov eax, dword ptr [esp + 4]
    mov dword ptr ds:[BSS_IntVector], eax
    
    /* Store Bios32RegsInput & Bios32RegsOutput */
    mov eax, dword ptr [esp + 8]
    mov dword ptr [Bios32RegsInput], eax
    mov eax, dword ptr [esp + 12]
    mov dword ptr [Bios32RegsOutput], eax

    /* Save registers */
    push ds
    push es
    push fs
    push gs
    pusha

    /* Copy input registers to BSS data */
    /* This is needed because realmode is a separate section */
    mov esi, dword ptr [Bios32RegsInput]
    mov edi, BSS_RegisterSet
    mov ecx, REGS_SIZE / 4

    /* Switch to real mode */
    mov dword ptr [_arch_continue], offset bios32_return
    jmp SwitchToRealMode

bios32_return:
    /* Copy output registers */
    mov esi, BSS_RegisterSet
    mov edi, dword ptr [Bios32RegsOutput]
    mov ecx, REGS_SIZE / 4
    rep movsd

    /* Restore all registers and segment registers */
    popa
    pop gs
    pop fs
    pop es
    pop ds

    ret