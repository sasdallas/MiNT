/**
 * @file libraries/hal/x86/hal_main.c
 * @brief Basic test header file.
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <WinDef.h>
#include <mintdefs.h>
#include <arc/arc.h>
#include <ndk/arch/i386/ke.h>
#include <xdk/ketypes.h>
#include <ddk/wdm.h>

CODE_SEG("init")
BOOLEAN
MINTHALAPI
HalInitSystem (IN ULONG BootPhase,
                IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    // Initialize the thingamajigger
    return TRUE;
}


KIRQL
MINTHALAPI
KeGetCurrentIrql (VOID)
{
    return 0;
}