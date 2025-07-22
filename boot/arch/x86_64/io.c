/**
 * @file boot/arch/x86_64/io.c
 * @brief I/O port interaction
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <arch/x86_64/io.h>

VOID IoWrite8(UINT32 Port, UINT8 Value) {
    __asm__ __volatile__("outb %b[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

VOID IoWrite16(UINT32 Port, UINT16 Value) {
    __asm__ __volatile__("outw %w[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

VOID IoWrite32(UINT32 Port, UINT32 Value) {
    __asm__ __volatile__("outl %k[Data], %w[Port]" :: [Port] "Nd" (Port), [Data] "a" (Value));
}

UINT8 IoRead8(UINT32 Port) {
    UINT8 Output;
    __asm__ __volatile__("inb %w[Port], %b[Output]" : [Output] "=a" (Output) : [Port] "Nd"(Port));
    return Output;
}

UINT16 IoRead16(UINT32 Port) {
    UINT16 Output;
    __asm__ __volatile__("inw %w[Port], %w[Output]" : [Output] "=a" (Output) : [Port] "Nd"(Port));
    return Output;
}

UINT32 IoRead32(UINT32 Port) {
    UINT32 Output;
    __asm__ __volatile__("inl %w[Port], %k[Output]" : [Output] "=a" (Output) : [Port] "Nd"(Port));
    return Output;
}