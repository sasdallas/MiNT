/**
 * @file mintkrnl/ki/arch/x86_64/init.c
 * @brief x86_64 initialization code
 * 
 * 
 * @copyright
 * This file is part of the Hexahedron kernel, which is part of the Ethereal Operating System.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 Samuel Stuart
 */

#include <winnt.h>
#include <ntdef.h>
#include <hal.h>
#include <kddll.h>

extern BOOL NTAPI HalInitSystem(ULONG BootPhase, PVOID LoaderBlock);

DECLSPEC_NORETURN void __stdcall KiSystemStartup() {
    /* Test */
    HalInitSystem(0, NULL);
    KdDebuggerInitialize0(NULL);

    KdpDebugPrint("MINTKRNL v1.0.0\n");
    KdpDebugPrint("KdDebuggerInitialize0 success\n");

    for (;;);
    __builtin_unreachable();
}