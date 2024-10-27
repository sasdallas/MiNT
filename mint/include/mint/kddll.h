/**
 * @file mint/include/mint/kddll.h
 * @brief Kernel debugger DLL header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system
 * However, it has been released into the public domain and is free to use
 * This file comes with no warranty.
 */

#pragma once

/** Includes **/
#include <psdk/ntstatus.h>
#include <arc/arc.h>
#include <xdk/ntdef.h>
#include <mintdefs.h>
#include <WinDef.h>
#include <kddbg.h>


/** Functions **/

/* Phase 0 initialize */
NTSTATUS
MINTAPI
KdDebuggerInitialize0(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
);


/* Phase 1 initialize */
NTSTATUS
MINTAPI
KdDebuggerInitialize1(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
);


/* Send packet */
VOID
MINTAPI
KdSendPacket(
    IN ULONG PacketType,
    IN PKD_BUFFER FirstBuffer,
    IN OPTIONAL PKD_BUFFER SecondBuffer,
    IN OUT PKD_CONTEXT KdContext
);

/* Receive a packet */
KDSTATUS
MINTAPI
KdReceivePacket(
    IN ULONG PacketType,
    IN OUT OPTIONAL PKD_BUFFER FirstBuffer,
    IN OUT OPTIONAL PKD_BUFFER SecondBuffer,
    OUT OPTIONAL PULONG PayloadBytes,
    IN OUT OPTIONAL PKD_CONTEXT KdContext
);

