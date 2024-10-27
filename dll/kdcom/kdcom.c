/**
 * @file libraries/kdcom/kdcom.c
 * @brief Kernel debugger main file
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <WinDef.h>
#include <arc/arc.h>
#include <xdk/ntdef.h>
#include <cportlib/cport.h>

/* Variables */
CPPORT KdComPort;


/* COM port number -> I/O port */
const ULONG PortArray[] = {
    0, 0x3F8, 0x2F8, 0x3E8, 0x2E8
};


/* Print a string */
NTSTATUS
MINTAPI
DECLSPEC_EXPORT
KdpDbgPrint
(
    PCSTR Format
)
{
    /* Only format is supported due to lack of vsnprintf */
    PCHAR Buffer = Format;
    while (*Buffer)
    {
        CpPutByte(&KdComPort, *Buffer++);
    }
}

/* Initialize a port */
NTSTATUS
MINTAPI
DECLSPEC_EXPORT
KdpPortInitialize(
    IN ULONG ComPortNumber,
    IN ULONG ComPortBaudRate
)
{
    NTSTATUS Status;

    Status = CpInitialize(&KdComPort,
                            (void*)(ULONG_PTR)PortArray[ComPortNumber],
                            ComPortBaudRate);

    /* Check for success */
    if (!NT_SUCCESS(Status))
    {
        return STATUS_INVALID_PARAMETER;
    }
    else
    {
        KdpDbgPrint("MiNTy fresh!");
        return STATUS_SUCCESS;
    }
}


/* Phase 0 initialize */
NTSTATUS
MINTAPI
DECLSPEC_EXPORT
KdDebuggerInitialize0(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
)
{
    /* Start the serial port */
    ULONG ComPortNumber = 1; /* COM1 */
    ULONG ComPortBaudRate = 1;  

    return KdpPortInitialize(ComPortNumber, ComPortBaudRate);
}

/* Phase 1 initialize */ 
NTSTATUS
MINTAPI
DECLSPEC_EXPORT
KdDebuggerInitialize1(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
)
{

}