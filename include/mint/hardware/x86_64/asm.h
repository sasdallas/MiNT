/**
 * @file include/mint/hardware/x86_64/asm.h
 * @brief ASM includes
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#ifndef __MINT_HARDWARE_X86_64_ASM_H__
#define __MINT_HARDWARE_X86_64_ASM_H__

#define KTRAP_Size          0x0160
#define KTRAP_Rax           0x30
#define KTRAP_Rcx           0x38
#define KTRAP_Rdx           0x40
#define KTRAP_R8            0x48
#define KTRAP_R9            0x50
#define KTRAP_R10           0x58
#define KTRAP_R11           0x60
#define KTRAP_GsBase        0x68

#define KTRAP_Xmm0          0x70
#define KTRAP_Xmm1          0x80
#define KTRAP_Xmm2          0x90
#define KTRAP_Xmm3          0xA0
#define KTRAP_Xmm4          0xB0
#define KTRAP_Xmm5          0xC0

#define KTRAP_FaultAddress  0xD0

#define KTRAP_Dr0           0xD8
#define KTRAP_Dr1           0xE0
#define KTRAP_Dr2           0xE8
#define KTRAP_Dr3           0xF0
#define KTRAP_Dr6           0xF8
#define KTRAP_Dr7           0x100

#define KTRAP_SegDs         0x130
#define KTRAP_SegEs         0x132
#define KTRAP_SegFs         0x134
#define KTRAP_SegGs         0x136

#define KTRAP_TrapFrame     0x138
#define KTRAP_Rbx           0x140
#define KTRAP_Rdi           0x148
#define KTRAP_Rsi           0x150
#define KTRAP_Rbp           0x158

#define KTRAP_ErrorCode     0x160

#endif