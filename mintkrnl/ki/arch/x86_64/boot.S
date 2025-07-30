/**
 * @file mintkrnl/ki/arch/x86_64/boot.S
 * @brief Contains boot trampolines
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

.extern KiContinueFromNewKernelStack

.global KiSwitchStacks
KiSwitchStacks:
    mov %rsp, %rcx
    sub $0x308, %rsp
    jmp KiContinueFromNewKernelStack