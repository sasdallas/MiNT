/**
 * @file boot/arch/x86_64/serial.c
 * @brief Serial port initialization code
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <arch/x86_64/io.h>
#include <arch/x86_64/serial.h>
#include <arch/serial.h>
#include <ui.h>

/* Selected COM port */
ULONG IoPort = SERIAL_COM1_PORT;

VOID SerialSetBaudRate(ULONG BaudRate) {
    ULONG LineControlRegister;
    ULONG Mode = 115200 / BaudRate;

    /* Enable the DLAB */
    LineControlRegister = IoRead8(IoPort + SERIAL_PORT_LINECTRL);
    IoWrite8(IoPort + SERIAL_PORT_LINECTRL, LineControlRegister | 0x80);

    /* Set the baud rate */
    IoWrite8(IoPort + SERIAL_PORT_DIVLSB, (UCHAR)(Mode & 0xFF));
    IoWrite8(IoPort + SERIAL_PORT_DIVMSB, (UCHAR)((Mode >> 8) & 0xFF));

    /* Reset the DLAB */
    IoWrite8(IoPort + SERIAL_PORT_LINECTRL, LineControlRegister);
}

INT SerialInitialize(ULONG ComPort, ULONG BaudRate) {
    /* Get COM port */
    switch (ComPort) {
        case 1:
            /* COM1 */
            IoPort = SERIAL_COM1_PORT;
            break;
        case 2:
            /* COM2 */
            IoPort = SERIAL_COM2_PORT;
            break;
        case 3:
            /* COM3 */
            IoPort = SERIAL_COM3_PORT;
            break;
        case 4:
            /* COM4 */
            IoPort = SERIAL_COM4_PORT;
            break;
        default:
            /* Assume COM1 */
            IoPort = SERIAL_COM1_PORT;
            break;
    }

    /* Disable all interrupts */
    IoWrite8(IoPort + SERIAL_PORT_INTENABLE, 0);
    
    /* Set the baud rate */
    SerialSetBaudRate(BaudRate);

    /* Use 8 bits, no parity, with one stop bit */
    IoWrite8(IoPort + SERIAL_PORT_LINECTRL, 0x03);

    /* Enable FIFO */
    IoWrite8(IoPort + SERIAL_PORT_FIFOCTRL, 0xC7);

    /* Enable IRQs, RTS/DSR */
    IoWrite8(IoPort + SERIAL_PORT_MODEMCTRL, 0x0F);

    return 0;
}

VOID SerialPrintCharacter(CHAR Character) {
    while (!(IoRead8(IoPort + SERIAL_PORT_LINESTAT) & 0x20));

    IoWrite8(IoPort + SERIAL_PORT_WRITEBUF, Character);
}

INT SerialPrintCallback(PCHAR String, SIZE_T Length) {
    for (SIZE_T i = 0; i < Length; i++) {
        SerialPrintCharacter(String[i]);
    }

    return Length;
}

VOID SerialPrint(PCHAR Format, ...) {
    va_list ap;
    va_start(ap, Format);
    UiPrintCallback(SerialPrintCallback, Format, ap);
    va_end(ap);
}