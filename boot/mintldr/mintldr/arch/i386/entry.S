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
    /* Setup segments */
    mov ax, PMODE_DS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    /* No idea why we preload this, but it works... */
    lidt idtptr
    call _IdtInit

    call _MintStart

    cli
    hlt

idtptr:
    .word 255
    .long _i386Idt