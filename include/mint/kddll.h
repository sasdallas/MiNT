/**
 * @file include/mint/kddll.h
 * @brief Kernel debugger DLL definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_KDDLL_H__
#define __MINT_KDDLL_H__ 1

__MINT_BEGIN_DECLS

#include <winnt.h>
#include <ntstatus.h>
#include <windbgkd.h>


/* NOT OFFICIAL, MINTKRNL INTERNAL */
NTAPI
NTSTATUS
KdpDebugPrint(
    PCHAR Format,
    ...
);

NTAPI
VOID
KdSendPacket(
    IN ULONG PacketType,
    IN PSTRING MessageHeader,
    IN PSTRING MessageData,
    IN OUT PKD_CONTEXT Context
);


NTAPI
KDSTATUS
KdReceivePacket(
    IN ULONG PacketType,
    OUT PSTRING MessageHeader,
    OUT PSTRING MessageData,
    OUT PULONG DataLength,
    IN OUT PKD_CONTEXT Context
);


NTAPI
NTSTATUS
KdDebuggerInitialize0(
    IN OPTIONAL PVOID LoaderParameterBlock
);

NTAPI
NTSTATUS
KdDebuggerInitialize1(
    IN OPTIONAL PVOID LoaderParameterBlock
);

__MINT_END_DECLS

#endif 