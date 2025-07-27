/**
 * @file lib/serial/serial.c
 * @brief Serial port library
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <libraries/serial/serial.h>
#include <hal.h>

NTAPI
NTSTATUS
SerialSetBaudRate(
    IN PSERIAL_PORT Port,
    IN ULONG BaudRate
)
{
    if (SERIAL_CLOCK_RATE % BaudRate) {
        return STATUS_INVALID_PARAMETER;
    }

    ULONG LineControlRegister;
    ULONG Mode = SERIAL_CLOCK_RATE / BaudRate;

    /* Enable the DLAB */
    LineControlRegister = READ_PORT_UCHAR((PVOID)(Port->IoPort + SERIAL_PORT_LINECTRL));
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_LINECTRL), LineControlRegister | 0x80);

    /* Set the baud rate */
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_DIVLSB), (UCHAR)(Mode & 0xFF));
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_DIVMSB), (UCHAR)((Mode >> 8) & 0xFF));

    /* Reset the DLAB */
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_LINECTRL), LineControlRegister);

    Port->BaudRate = BaudRate;
    return STATUS_SUCCESS;
}

NTAPI
NTSTATUS
SerialInitialize(
    IN PSERIAL_PORT Port,
    IN UINT32 ComPort 
) 
{
    if (!Port || !ComPort) {
        return STATUS_INVALID_PARAMETER;
    }

    if (SERIAL_CLOCK_RATE % Port->BaudRate) {
        return STATUS_INVALID_PARAMETER;
    }

    switch (ComPort) {
        case 1:
            Port->IoPort = SERIAL_COM1_PORT;
            break;

        case 2:
            Port->IoPort = SERIAL_COM2_PORT;
            break;

        case 3:
            Port->IoPort = SERIAL_COM3_PORT;
            break;

        default:
            return STATUS_INVALID_PARAMETER;
    }

    /* Disable port interrupts */
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_LINECTRL), 0);
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_INTENABLE), 0);

    /* Set the baud rate for the port */
    NTSTATUS BaudRateSuccess = SerialSetBaudRate(Port, Port->BaudRate);
    if (!NT_SUCCESS(BaudRateSuccess)) {
        return BaudRateSuccess;
    }

    /* Use 8 bits, no parity, with one stop bit */
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_LINECTRL), 0x03);

    /* Enable FIFO */
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_FIFOCTRL), 0xC7);

    /* Enable IRQs, RTS/DSR */
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_MODEMCTRL), 0x0F);

    return STATUS_SUCCESS;
}

NTAPI
VOID
SerialPutCharacter(
    IN PSERIAL_PORT Port,
    IN CHAR Character
) 
{
    while (!(READ_PORT_UCHAR((PVOID)(Port->IoPort + SERIAL_PORT_LINESTAT)) & 0x20));
    WRITE_PORT_UCHAR((PUCHAR)(Port->IoPort + SERIAL_PORT_WRITEBUF), Character);    
}