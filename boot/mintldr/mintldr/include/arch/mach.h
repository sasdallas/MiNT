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


/*** STRUCTURES ***/

/* Mainly just reserved. This could be a boolean */
typedef enum {
    MEM_USABLE = 0,
    MEM_NOT_USABLE = 1
} GENERIC_MEMORY_TYPE;

/* Normally memory maps (or at least their descriptors) are hardware-specific */
/* MiNT's memory manager requires descriptors to construct its own map */
/* Because of this, we will use a generic memory descriptor */
typedef struct _GENERIC_MEMORY_DESCRIPTOR
{
    ULONGLONG BaseAddress;
    ULONGLONG RegionSize;
    GENERIC_MEMORY_TYPE RegionType;
} GENERIC_MEMORY_DESCRIPTOR, *PGENERIC_MEMORY_DESCRIPTOR;





#endif