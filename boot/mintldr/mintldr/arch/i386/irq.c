/**
 * @file mintldr/arch/i386/irq.c
 * @brief IRQ handler for I386 platform
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <mintldr.h>
#include <ndk/arch/i386/ke.h>


/* Handler method */
void
__cdecl
i386ExceptionHandler(PKTRAP_FRAME TrapFrame) {
    for (;;);

}