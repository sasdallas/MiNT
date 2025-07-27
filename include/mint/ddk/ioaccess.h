/**
 * @file include/mint/ddk/ioaccess.h
 * @brief I/O access
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_IOACCESS_H__
#define __MINT_IOACCESS_H__ 1

__MINT_BEGIN_DECLS

#include <hal.h>
#include <winnt.h>

NTHALAPI
VOID
NTAPI
WRITE_PORT_UCHAR(
    IN PUCHAR Port,
    IN UCHAR Value
);

NTHALAPI
VOID
NTAPI
WRITE_PORT_USHORT(
    IN PUSHORT Port,
    IN USHORT Value
);

NTHALAPI
VOID 
NTAPI
WRITE_PORT_ULONG(
    IN PULONG Port,
    IN ULONG Value
);

NTHALAPI
UCHAR
NTAPI
READ_PORT_UCHAR(
    IN PVOID Port
);

NTHALAPI
USHORT
NTAPI
READ_PORT_USHORT(
    IN PVOID Port
);

NTHALAPI
ULONG
NTAPI
READ_PORT_ULONG(
    IN PVOID Port
);



__MINT_END_DECLS

#endif