/**
 * @file mintldr/arch/i386/mach/serial.c
 * @brief Serial interface code
 * 
 * This code is specified to x86 architecture and programs in COM1 UART.
 * @note The mach interface itself is provided here.
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */


#include <arch/i386/hal.h>
#include <arch/i386/mach.h>
#include <mintldr.h>

STATIC UINT IoPort = MACH_I386_COM1_PORT; /* I/O address of serial port */

STATIC
VOID
MachSendSerialCharacter(
    CHAR c
);

/* Initialize the serial system (and test it). This is specific to mach internally */
VOID
MachInitializeSerialController(
    UINT ComPort,
    UINT BaudRate
)
{
    

    /* Handle ComPort */
    switch (ComPort)
    {
        case 1:
            /* COM1 */
            IoPort = MACH_I386_COM1_PORT;
            break;
        case 2:
            /* COM2 */
            IoPort = MACH_I386_COM2_PORT;
            break;
        case 3:
            /* COM3 */
            IoPort = MACH_I386_COM3_PORT;
            break;
        case 4:
            /* COM4 */
            IoPort = MACH_I386_COM4_PORT;
            break;
        default:
            /* Most of the ports further beyond here are at I/O addresses in memory that can vary */
            /* tinyhal isn't advanced enough yet to check BDA so imma be a lazy coder */
            IoPort = MACH_I386_COM1_PORT;
            break;
    }

    UiPrint("MACH: Preparing serial port 0x%x\n", IoPort);

    /**** WARNING WARNING WARNING WARNING WARNING WARNING ****/
    /**** THIS CODE DOES NOT WORK. I DO NOT KNOW WHY. I AM NOT SURE WHY ****/
    /**** HOWEVER, NOT CONFIGURING THE SERIAL PORT DOES ALLOW US TO OUTPUT ON EMULATORS ****/
    /**** DISABLE ONLY IF YOU KNOW WHAT YOU ARE DOING. ****/
    return;

    /* BaudRate is set my MSB/LSB in divisors */
    UINT Divisor = 115200 / BaudRate;

    /** Initialization process **/
    /* TODO: These should be constantized and bitmasked */

    /* Disable interrupts */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_INTENABLE, 0x00);
    
    /* Enable DLAB bit */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_LINECTRL, 0x80);

    /* Write divisors */
    /* im not a math guy :( THIS WILL 100% BREAK ON BAUD RATES OF LENGTH >1 BYTE */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_DIVLSB, Divisor);
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_DIVMSB, 0x0);

    /* Configure bits/parity/stop bits */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_LINECTRL, 0xC7);

    /* Setup MODEMCTRL: Enable IRQs, request to send, and data terminal ready */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_MODEMCTRL, 0x0B);

    /** Chip testing **/
    
    /* Reconfigure in loopback mode */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_MODEMCTRL, 0x1E);

    /* Send byte 0xFF */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_WRITEBUF, 0xFF);

    /* Did we get the same byte back? */
    if (READ_PORT_UCHAR(IoPort + MACH_I386_SERIAL_RECVBUF) != 0xFF)
    {
        /* Faulty port */
        IoPort = 0x0;
        return;
    }

    /* Reset in normal operations mode */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_MODEMCTRL, 0x0F);
}

STATIC
VOID
MachSendSerialCharacter(
    CHAR c
)
{
    /* Check the transmit buffer */
    while (!(READ_PORT_UCHAR(IoPort + MACH_I386_SERIAL_LINESTAT) & 0x20));
    
    /* Write character */
    WRITE_PORT_UCHAR(IoPort + MACH_I386_SERIAL_WRITEBUF, c);
}

/* Receive a character */
/* WARNING: You can hang the system if you're not careful */
STATIC
CHAR
MachRecieveSerialCharacter()
{
    /* Check if we've received anything */
    while (!(READ_PORT_UCHAR(IoPort + MACH_I386_SERIAL_LINESTAT) & 1));

    /* Return character */
    return READ_PORT_UCHAR(IoPort + MACH_I386_SERIAL_RECVBUF);
}

/* Data print method */
STATIC
BOOL
MachPrintSerialData(
    PCSTR Data,
    SIZE_T Length
)
{
    
    for (SIZE_T i = 0; i < Length; i++) 
    {
        MachSendSerialCharacter(Data[i]);
    }

    return TRUE;
}

/* Print line to serial */
INT
MachPrintSerial(
    PCSTR Format,
    ...
)
{
    va_list ap;
    va_start(ap, Format);
    INT ReturnValue = UiPrintCallback((MachPrintSerialData), Format, ap);
    va_end(ap);
    return ReturnValue;
}