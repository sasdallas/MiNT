/**
 * @file mint/include/mint/xdk/iofuncs.h
 * @brief I/O functions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system
 * However, it has been released into the public domain and is free to use
 * This file comes with no warranty.
 */

#pragma once

#ifndef _XDK_IOFUNCS_H
#define _XDK_IOFUNCS_H

#include <WinDef.h>
#include <ddk/wdm.h>

/* Write 8-bit value */
VOID
MINTHALAPI
WRITE_PORT_UCHAR(
    IN UINT Port,
    IN UCHAR Value
);

/* Write 16-bit value */
VOID
MINTHALAPI
WRITE_PORT_USHORT(
    IN UINT Port,
    IN USHORT Value
);

/* Write 32-bit value */
VOID
MINTHALAPI
WRITE_PORT_ULONG(
    IN UINT Port,
    IN ULONG Value
);

/* Read 8-bit value */
UCHAR
MINTHALAPI
READ_PORT_UCHAR(
    IN UINT Port
);

/* Read 16-bit value */
USHORT
MINTHALAPI
READ_PORT_USHORT(
    IN UINT Port
);

/* Read 32-bit value */
ULONG
MINTHALAPI
READ_PORT_ULONG(
    IN UINT Port
);


#endif