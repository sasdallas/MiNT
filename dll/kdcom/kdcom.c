/**
 * @file dll/kdcom/kdcom.c
 * @brief KDCOM debugger
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <kddll.h>
#include <libraries/serial/serial.h>
#include <stdio.h>

/* Default kernel debugger port */
static SERIAL_PORT KdpDefaultDebugPort = { .BaudRate = 38400, };

NTAPI
NTSTATUS
KdDebuggerInitialize0(
    IN OPTIONAL PVOID LoaderParameterBlock
)
{
    NTSTATUS Status;
    
    Status = SerialInitialize(&KdpDefaultDebugPort, 1);
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    return STATUS_SUCCESS;
}


NTAPI
NTSTATUS
KdpDebugPrint(
    PCHAR Format,
    ...
)
{
    char str[512] = { 0 };
    va_list ap;
    va_start(ap, Format);
    vsnprintf(str, 512, Format, ap);
    va_end(ap);

    char *s = str;
    while (*s) {
        SerialPutCharacter(&KdpDefaultDebugPort, *s);
        s++;
    }

    return STATUS_SUCCESS;
}