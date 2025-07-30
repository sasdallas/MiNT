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
#include <ke.h>
#include <arc.h>

extern BOOL NTAPI HalInitSystem(ULONG BootPhase, PVOID LoaderBlock);
extern VOID KiSwitchStacks(IN UINT_PTR Stack);

/* Loader block */
PLOADER_PARAMETER_BLOCK KeLoaderBlock = NULL;

DECLSPEC_NORETURN
VOID
KiContinueFromNewKernelStack()
{
    KdpDebugPrint("KiContinueFromNewKernelStack Landed\n");
    for (;;);
}

DECLSPEC_NORETURN
VOID
NTAPI
KiSystemStartup(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
) 
{
    if (!KeLoaderBlock) {
        KeLoaderBlock = LoaderBlock;
    }
    
    KdDebuggerInitialize0(LoaderBlock);
    KdpDebugPrint("================================================\n");
    KdpDebugPrint("MINTKRNL v1.0.0\n");
    KdpDebugPrint("(C) The MiNT operating system, 2025\n\n");

    KdpDebugPrint("Booted from ARC partition: %s\n", LoaderBlock->ArcBootDeviceName);

    KeInitExceptions();

    KiSwitchStacks(LoaderBlock->KernelStack);

    for (;;);
    __builtin_unreachable();
}