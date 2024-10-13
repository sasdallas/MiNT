// =======================================================================================================
// FILE:         i386.S
// DESCRIPTION:  i386 real mode entrypoint
// AUTHOR:       Samuel S.
// CREATED:      October 13th, 2024
// LICENSE:      Under the license of MiNT (BSD 3-clause)
// =======================================================================================================

#include <asm.inc>
#include <pecommon.h> // Common PE header definitions
#include <reactos/x86common.h>  // Should make this a system-wide file instead of being part of ReactOS.




.code16

.global start
start:
    .byte HEX(e9), HEX(fd), HEX(01)

.org 512
.global _rm_entrypoint
_rm_entrypoint:
    // Good to be back, setup segment registers as bootloader might've messed with them.
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    // Setup a 16-bit stack
    mov sp, word ptr ds:[HEX(6F00)] // x86common says to do this


    call enableA20_KKbrd_Out

    // pmode preparations take place in JumpToProtectedMode
    mov ax, FREELDR_PE_BASE / 16
    mov es, ax

    // Get address of optional header
    mov eax, dword ptr es:[IMAGE_DOS_HEADER_e_lfanew]
    add eax, 4 + IMAGE_FILE_HEADER_SIZE

    // Get entrypoint address
    mov eax, dword ptr es:[eax + IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]
    add eax, FREELDR_PE_BASE // Account for PE base

    // Save the entrypoint
    mov dword ptr ds:[pe_entrypoint], eax

    // Restore ES back to its zeroed state
    xor ax, ax
    mov es, ax
    jmp JumpToProtectedMode

JumpToProtectedMode:
    cli

    call installGDT
    

    // Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp far ptr PMODE_CS:pmode_entrypoint



// It isn't good practice to flip flop between architectures, but to me it seems like even worse practice to do this.
// The protected mode method will encode an indirect jump.
pmode_entrypoint:
    .byte HEX(0ff), HEX(25) // opcode of indirect jump
    // Specify pe_entrypoint
    .word pe_entrypoint, 0

pe_entrypoint:
    .long 0 // When saving entrypoint this'll get overwritten.
    nop // Random opcode


// Includes
#include "a20.inc"
#include "gdt.inc"


.org (FREELDR_PE_BASE - FREELDR_BASE - 1)
.byte 0
