/**
 * @file mintldr/include/arch/i386/mach.h
 * @brief Architecture specific mach definitions
 * 
 * @warning This file is specific to the I386 architecture.
 * @note The mach system may be confusing. There's a global file in arch/mach.h that lists definitions
 *       which the more generic code (e.g. main mintldr code) will go off of.
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef ARCH_MACH_H
#define ARCH_MACH_H

/* Serial port defintions */
#define MACH_I386_COM1_PORT 0x3F8
#define MACH_I386_COM2_PORT 0x2F8
#define MACH_I386_COM3_PORT 0x3E8
#define MACH_I386_COM4_PORT 0x2E8
#define MACH_I386_COM5_PORT 0x5F8
#define MACH_I386_COM6_PORT 0x4F8
#define MACH_I386_COM7_PORT 0x5E8
#define MACH_I386_COM8_PORT 0x4E8

/* Serial chip definitions (in terms of I/O port offset) */
#define MACH_I386_SERIAL_RECVBUF    0   /* Receive buffer */
#define MACH_I386_SERIAL_WRITEBUF   0   /* Write buffer */
#define MACH_I386_SERIAL_INTENABLE  1   /* Interrupt enable */
#define MACH_I386_SERIAL_DIVLSB     0   /* (DLAB = 1) LSB of divisor */
#define MACH_I386_SERIAL_DIVMSB     1   /* (DLAB = 1) MSB of divisor */
#define MACH_I386_SERIAL_INTID      2   /* Interrupt identification */
#define MACH_I386_SERIAL_FIFOCTRL   2   /* FIFO control registers */
#define MACH_I386_SERIAL_LINECTRL   3   /* Line control register */
#define MACH_I386_SERIAL_MODEMCTRL  4   /* Modem control register */
#define MACH_I386_SERIAL_LINESTAT   5   /* Line status register */
#define MACH_I386_SERIAL_MODEMSTAT  6   /* Modem status register */
#define MACH_I386_SERIAL_SCRATCH    7   /* Scratch register (used for testing) */

/* PIC definitions */
#define MACH_I386_PIC1_IO           0x20    /* Master PIC */
#define MACH_I386_PIC2_IO           0xA0    /* Slave PIC */
#define MACH_I386_PIC_CMD           0       /* PIC Command (offset) */
#define MACH_I386_PIC_DATA          1       /* PIC Data (offset) */

/* PIC commands */
#define MACH_I386_ICW1_ICW4_PRES    0x01    /* Indicate ICW4 is present */
#define MACH_I386_ICW1_SINGLE_MODE  0x02    /* Single (aka cascade) mode */
#define MACH_I386_ICW1_INTERVAL4    0x04    /* Call address interval 4 */
#define MACH_I386_ICW1_LEVEL        0x08    /* Level triggered */
#define MACH_I386_ICW1_INIT         0x10    /* Initialization */

#define MACH_I386_ICW4_8086MODE     0x01    /* 8086/88 mode */
#define MACH_I386_ICW4_AUTO         0x02    /* Auto EOI */
#define MACH_I386_ICW4_BUF_SLAVE    0x08    /* Buffered mode in slave */
#define MACH_I386_ICW4_BUF_MASTER   0x0C    /* Buffered mode in master */
#define MACH_I386_ICW4_SFNM         0x10    /* Special fully nested */


/* Memory Map Types */
typedef enum
{
    MEM_USABLE              = 1,
    MEM_RESERVED            = 2,
    MEM_ACPI_RECLAIMABLE    = 3,
    MEM_ACPI_NVS            = 4,
    MEM_BAD_MEMORY          = 5
} BIOS_MEMORY_TYPE;

/* Memory Map */
typedef struct _BIOS_MEMORY_MAP
{
    union
    {
        ULONGLONG BaseAddress;
        struct {
            ULONG BaseAddressLow;
            ULONG BaseAddressHigh;
        } ba;
    };


    union
    {
        ULONGLONG RegionLength;
        struct {
            ULONG RegionLengthLow;
            ULONG RegionLengthHigh;
        } rl;
    };
 
    ULONG RegionType; /* See enum BIOS_MEMORY_TYPE */

    /* Part of ACPI 3.0 standard */
    struct _ACPI_DATA
    {
        ULONG IgnoreEntry:1; /* Do not use */
        ULONG Nonvolatile:1; /* Nonvolatile. Requires further probing */

        ULONG Reserved:30; /* TODO: Check if all of these are reserved. Seen information that they aren't in later ACPI versions */
    } AcpiData;
} BIOS_MEMORY_MAP, *PBIOS_MEMORY_MAP;




#endif