/**
 * @file include/libraries/serial/serial.h
 * @brief COM port library header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_SERIAL_SERIAL_H__
#define __MINT_SERIAL_SERIAL_H__

__MINT_BEGIN_DECLS

#include <winnt.h>
#include <ntstatus.h>

#define SERIAL_CLOCK_RATE           115200

#define SERIAL_COM1_PORT 0x3F8
#define SERIAL_COM2_PORT 0x2F8
#define SERIAL_COM3_PORT 0x3E8
#define SERIAL_COM4_PORT 0x2E8
#define SERIAL_COM5_PORT 0x5F8
#define SERIAL_COM6_PORT 0x4F8
#define SERIAL_COM7_PORT 0x5E8
#define SERIAL_COM8_PORT 0x4E8

/* Serial chip definitions (in terms of I/O port offset) */
#define SERIAL_PORT_RECVBUF    0   /* Receive buffer */
#define SERIAL_PORT_WRITEBUF   0   /* Write buffer */
#define SERIAL_PORT_INTENABLE  1   /* Interrupt enable */
#define SERIAL_PORT_DIVLSB     0   /* (DLAB = 1) LSB of divisor */
#define SERIAL_PORT_DIVMSB     1   /* (DLAB = 1) MSB of divisor */
#define SERIAL_PORT_INTID      2   /* Interrupt identification */
#define SERIAL_PORT_FIFOCTRL   2   /* FIFO control registers */
#define SERIAL_PORT_LINECTRL   3   /* Line control register */
#define SERIAL_PORT_MODEMCTRL  4   /* Modem control register */
#define SERIAL_PORT_LINESTAT   5   /* Line status register */
#define SERIAL_PORT_MODEMSTAT  6   /* Modem status register */
#define SERIAL_PORT_SCRATCH    7   /* Scratch register (used for testing) */


typedef struct _SERIAL_PORT {
    ULONG BaudRate;
    ULONG IoPort;
} SERIAL_PORT, *PSERIAL_PORT;


NTAPI
NTSTATUS
SerialInitialize(
    IN PSERIAL_PORT Port,
    IN UINT32 ComPort 
);


NTAPI
VOID
SerialPutCharacter(
    IN PSERIAL_PORT Port,
    IN CHAR Character
);

__MINT_END_DECLS

#endif