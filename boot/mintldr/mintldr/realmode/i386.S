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

/* The start code will encode a jump to _rm_entrypoint */
.global start
start:
    .byte HEX(e9), HEX(fd), HEX(01)

.org 512
.global _rm_entrypoint
_rm_entrypoint:
    /* Good to be back, setup segment registers as bootloader might've messed with them. */
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    /* Setup a 16-bit stack */
    mov sp, word ptr ds:[HEX(6F00)]

    /* Enable A20 line */
    call enableA20_KKbrd_Out

    /* Save real mode entrypoint */
    mov dword ptr ds:[BSS_RealModeEntry], offset MoveToRealMode

    /* pmode preparations take place in JumpToProtectedMode */
    mov ax, FREELDR_PE_BASE / 16
    mov es, ax

    /* Get address of optional header */
    mov eax, dword ptr es:[IMAGE_DOS_HEADER_e_lfanew]
    add eax, 4 + IMAGE_FILE_HEADER_SIZE

    /* Get entrypoint address */
    mov eax, dword ptr es:[eax + IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]
    add eax, FREELDR_PE_BASE // Account for PE base

    /* Save the entrypoint */
    mov dword ptr ds:[pe_entrypoint], eax

    /* Restore ES back to its zeroed state */
    xor ax, ax
    mov es, ax
    jmp JumpToProtectedMode

/* arch/i386/entry.S will jump here during a BIOS32 call because we set it in BSS */
MoveToRealMode:
    /* Setup RMODE_DS, temporary. real mode segments should be nulled at RealMode code */
    mov ax, RMODE_DS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ds, ax

    /* Disable protected mode */
    mov eax, cr0
    and eax, CR0_PE_CLR
    mov cr0, eax
    jmp far ptr 0:RealMode

RealMode:
    /* Okay, now properly setup segments */
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ds, ax
    
    /* Restore real mode stack */
    mov sp, word ptr ds:[stack16]

    /* Load real mode IDT & enable interrupts */
    lidt rmode_idtptr
    sti

    /* Call our RealModeBios32 handler */
    /* BUG: Stack isn't being properly done. RealModeBios32 DOES NOT use ret, it just jumps to JumpToProtectedMode. */
    /* But not calling doesn't work... */
    call RealModeBios32    

    /* We're done, so we'll just head to JumpToProtectedMode to get back */
.global JumpToProtectedMode
JumpToProtectedMode:
    cli

    /* Save stack */
    mov word ptr ds:[stack16], sp

    /* Install GDT */
    call installGDT
    
    /* Enable protected mode */
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp far ptr PMODE_CS:pmode_entrypoint



/* It isn't good practice to flip flop between architectures, so do this.*/
/* The protected mode method will encode an indirect jump. */
pmode_entrypoint:
    .byte HEX(0ff), HEX(25) /* opcode of indirect jump */
    .word pe_entrypoint, 0 

pe_entrypoint:
    .long 0 /* PE entrypoint will be set */
    nop 


/* TODO: Two copies?? What are we, madmen? */
stack16:
    .word HEX(6F00)
    
/* How the hell does this work? */
rmode_idtptr:
    .word HEX(3FF)
    .long 0

/* Includes */
#include "a20.inc"
#include "gdt.inc"
#include "bios32.inc"

.org (FREELDR_PE_BASE - FREELDR_BASE - 1)
.byte 0
