/**
 * @file mintldr/include/arch/i386/hal.h
 * @brief Global definitions for the (tiny) HAL
 * 
 * @warning This file should never be used outside of i386 architecture.
 *          It contains definitions for functions and variables that are arch-specific.
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef ARCH_HAL_H
#define ARCH_HAL_H

#include <ddk/wdm.h>
#include <WinDef.h>
#include <arch/i386/i386.h>

/* Write 8-bit value */
VOID
WRITE_PORT_UCHAR(
    IN UINT Port,
    IN UCHAR Value
);

/* Write 16-bit value */
VOID
WRITE_PORT_USHORT(
    IN UINT Port,
    IN USHORT Value
);

/* Write 32-bit value */
VOID
WRITE_PORT_ULONG(
    IN UINT Port,
    IN ULONG Value
);

/* Read 8-bit value */
UCHAR
READ_PORT_UCHAR(
    IN UINT Port
);

/* Read 16-bit value */
UCHAR
READ_PORT_USHORT(
    IN UINT Port
);

/* Read 32-bit value */
UCHAR
READ_PORT_ULONG(
    IN UINT Port
);




#endif