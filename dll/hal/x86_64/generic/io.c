/**
 * @file dll/hal/x86_64/io.c
 * @brief HAL I/O functions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <hal.h>

NTHALAPI
VOID
NTAPI
WRITE_PORT_UCHAR(
    IN PUCHAR Port,
    IN UCHAR Value
)
{
    asm volatile ("outb %b[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

NTHALAPI
VOID
NTAPI
WRITE_PORT_USHORT(
    IN PUSHORT Port,
    IN USHORT Value
)
{
    asm volatile ("outw %w[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

NTHALAPI
VOID 
NTAPI
WRITE_PORT_ULONG(
    IN PULONG Port,
    IN ULONG Value
)
{
    asm volatile ("outl %k[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

NTHALAPI
UCHAR
NTAPI
READ_PORT_UCHAR(
    IN PVOID Port
)
{
    UCHAR Output;
    asm volatile ("inb %w[Port], %b[Output]" : [Output] "=a"(Output) : [Port] "Nd" (Port));
    return Output;
}

NTHALAPI
USHORT
NTAPI
READ_PORT_USHORT(
    IN PVOID Port
)
{
    USHORT Output;
    asm volatile ("inw %w[Port], %w[Output]" : [Output] "=a"(Output) : [Port] "Nd" (Port));
    return Output;
}

NTHALAPI
ULONG
NTAPI
READ_PORT_ULONG(
    IN PVOID Port
)
{
    ULONG Output;
    asm volatile ("inl %w[Port], %k[Output]" : [Output] "=a"(Output) : [Port] "Nd" (Port));
    return Output;
}