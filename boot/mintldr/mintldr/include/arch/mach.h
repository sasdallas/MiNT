/**
 * @file mintldr/include/arch/mach.h
 * @brief mach (Machine) interface
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef ARCH_MACH_H
#define ARCH_MACH_H

#include <WinDef.h>


/*** FUNCTIONS ***/

/* Initialize mach interface */
VOID
MachInit();

/* Print line to serial */
INT
MachPrintSerial(
    PCSTR Format,
    ...
);





#endif