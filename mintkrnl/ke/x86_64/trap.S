/**
 * @file mintkrnl/ke/x86_64/trap.S
 * @brief Trap functions for x86_64
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

.macro PUSH_TRAP_REGISTERS
    pushq %rbp 
    pushq %rsi
    pushq %rdi
    pushq %rbx
.endm