/**
 * @file mintldr/arch/i386/mach/mach.c
 * @brief Machine library for i386 architecture
 * 
 * This file provides all generalized functions and serves as a mini HAL.
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <arch/mach.h>


EXTERN
VOID
MachInitializeSerialController(
    UINT ComPort,
    UINT BaudRate
);


/* Initialize mach interface */
VOID
MachInit() 
{
    MachInitializeSerialController(
        1, /* COM1 */
        38400 /* 38400 baud */
    );
}


