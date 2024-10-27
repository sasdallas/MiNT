/**
 * @file mint/include/libraries/cportlib/cport.h
 * @brief COM port libraries header
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#pragma once

#ifndef _CPORT_H
#define _CPORT_H

/* Includes */
#include <WinDef.h>
#include <psdk/ntstatus.h>

/* Typedefs */
typedef struct _CPPORT
{
    PUCHAR Address;
    ULONG BaudRate;
    USHORT Flags;
} CPPORT, *PCPPORT;


/* Functions */

VOID
MINTAPI
CpEnableFifo(
    IN PUCHAR Address,
    IN BOOLEAN Enabled
);

VOID
MINTAPI
CpSetBaud(
    IN PCPPORT Port,
    IN ULONG BaudRate
);

NTSTATUS
MINTAPI
CpInitialize(
    IN PCPPORT Port,
    IN PUCHAR Address,
    IN ULONG BaudRate
);

USHORT
MINTAPI
CpGetByte(
    IN PCPPORT Port,
    OUT PUCHAR Byte,
    IN BOOLEAN Wait,
    IN BOOLEAN Poll
);

VOID
MINTAPI
CpPutByte(
    IN PCPPORT Port,
    IN UCHAR Byte
);




#endif