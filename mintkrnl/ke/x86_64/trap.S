/**
 * @file mintkrnl/ke/x86_64/trap.S
 * @brief 
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <hardware/x86_64/asm.h>
#include <bugcodes.h>

.extern KeBugCheckWithFrame

.macro FatalBugcheck exception p1 p2 p3 p4 trapframe
    pushq %rbp
    movq %rsp, %rbp

    subq $48, %rsp

    # Move parameters into correct argument locations
    movq $\exception, %rcx       
    movq \p1, %rdx              
    movq \p2, %r8               
    movq \p3, %r9               
    movq \p4, 32(%rsp)          
    movq \trapframe, 40(%rsp)

    # Call the function
    call KeBugCheckWithFrame

    # This function does not return, but just in case
    addq $48, %rsp
    popq %rbp
    ret
.endm


.macro EnterTrap ktrap
    movq %rbp, KTRAP_Rbp(\ktrap)

    /* Save general purpose */
    movq %rax, KTRAP_Rax(\ktrap)
    movq %rcx, KTRAP_Rcx(\ktrap)
    movq %rdx, KTRAP_Rdx(\ktrap)
    movq %r8, KTRAP_R8(\ktrap)
    movq %r9, KTRAP_R9(\ktrap)
    movq %r10, KTRAP_R10(\ktrap)
    movq %r11, KTRAP_R11(\ktrap)

    /* Save more */
    movq %rdi, KTRAP_Rdi(\ktrap)
    movq %rsi, KTRAP_Rsi(\ktrap)
    movq %rbx, KTRAP_Rbx(\ktrap)

    /* TODO: debug registers, CPU stuff, gsbase, XMM, and more. */
.endm

.global KiDivisionByZeroException
.seh_proc KiDivisionByZeroException
KiDivisionByZeroException:
    /* Push dummy error code */
    push $0

    /* Allocate space for KTRAP + stack */
    subq $KTRAP_Size + 8 + 32, %rsp
    leaq 40(%rsp), %r12             /* r10 = PKTRAP */

    EnterTrap       %r12
    FatalBugcheck   KMODE_EXCEPTION_NOT_HANDLED, $1, $2, $3, $4, %r12
.seh_endproc