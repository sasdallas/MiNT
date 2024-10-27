/**
 * @file mintldr/arch/i386/tinyhal.c
 * @brief Tiny HAL library for i386
 * 
 * @todo This should be implemented in a system-wide HAL library that MINTLDR links to.
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <arch/i386/hal.h>
#include <ddk/wdm.h>
#include <WinDef.h>

/* Write 8-bit value */
VOID
WRITE_PORT_UCHAR(
    IN UINT Port,
    IN UCHAR Value
)
{
    __asm__ __volatile__("outb %b[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

/* Write 16-bit value */
VOID
WRITE_PORT_USHORT(
    IN UINT Port,
    IN USHORT Value
)
{
    __asm__ __volatile__("outw %w[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

/* Write 32-bit value */
VOID
WRITE_PORT_ULONG(
    IN UINT Port,
    IN ULONG Value
)
{
    __asm__ __volatile__("outl %k[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}


/* Read 8-bit value */
UCHAR
READ_PORT_UCHAR(
    IN UINT Port
)
{
    UCHAR Output;
    __asm__ __volatile__("inb %w[Port], %b[Output]" : [Output] "=a" (Output) : [Port] "Nd"(Port));
    return Output;
}

/* Read 16-bit value */
UCHAR
READ_PORT_USHORT(
    IN UINT Port
)
{
    USHORT Output;
    __asm__ __volatile__("inw %w[Port], %w[Output]" : [Output] "=a" (Output) : [Port] "Nd"(Port));
    return Output;
}

/* Read 32-bit value */
UCHAR
READ_PORT_ULONG(
    IN UINT Port
)
{
    ULONG Output;
    __asm__ __volatile__("inl %w[Port], %k[Output]" : [Output] "=a" (Output) : [Port] "Nd"(Port));
    return Output;
}