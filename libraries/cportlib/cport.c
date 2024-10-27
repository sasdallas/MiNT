/**
 * @file libraries/cportlib/cport.c
 * @brief COM port library
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <cportlib/cport.h>
#include <psdk/ntstatus.h>
#include <xdk/iofuncs.h>
#include <hardware/i386/serial.h>
#include <WinDef.h>




VOID
MINTAPI
CpEnableFifo(
    IN PUCHAR Address,
    IN BOOLEAN Enable
)
{  
    if (Enable) 
    {
        WRITE_PORT_UCHAR((UINT)Address + FIFO_CONTROL_REGISTER, SERIAL_FCR_ENABLE | SERIAL_FCR_RCVR_RESET | SERIAL_FCR_TXMT_RESET);   
    }
    else
    {  
        WRITE_PORT_UCHAR((UINT)Address + FIFO_CONTROL_REGISTER, SERIAL_FCR_DISABLE);
    }
}



VOID
MINTAPI
CpSetBaud(
    IN PCPPORT Port,
    IN ULONG BaudRate
)
{
    ULONG LineControlRegister; 
    ULONG Mode = CLOCK_RATE / BaudRate; /* Serial chips are clocked at 115200, and the divisor is used to divide that 115200 into the baud rate */
                                        /* LSB & MSB latches exist to do this */

    /* Enable DLAB */
    LineControlRegister = READ_PORT_UCHAR((UINT)Port->Address + LINE_CONTROL_REGISTER);
    WRITE_PORT_UCHAR((UINT)Port->Address + LINE_CONTROL_REGISTER, LineControlRegister | SERIAL_LCR_DLAB);

    /* Set baud rate */
    WRITE_PORT_UCHAR((UINT)Port->Address + DIVISOR_LATCH_LSB, (UCHAR)(Mode & 0xFF));
    WRITE_PORT_UCHAR((UINT)Port->Address + DIVISOR_LATCH_MSB, (UCHAR)((Mode >> 8) & 0xFF));

    /* Reset DLAB */
    WRITE_PORT_UCHAR((UINT)Port->Address + LINE_CONTROL_REGISTER, LineControlRegister);

    /* Update port baud rate */
    Port->BaudRate = BaudRate;
}


NTSTATUS
MINTAPI
CpInitialize(
    IN PCPPORT Port,
    IN PUCHAR Address,
    IN ULONG BaudRate
)
{
    if (!Port || !Address || !BaudRate)
    {
        return STATUS_INVALID_PARAMETER;
    }

    if (CLOCK_RATE % BaudRate != 0)
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* TODO: Test ports */

    /* Create the basic port data */
    Port->Address = Address;
    Port->BaudRate = 0;
    Port->Flags = 0;

    /* Disable port interrupts */
    WRITE_PORT_UCHAR((UINT)Address + LINE_CONTROL_REGISTER, 0);
    WRITE_PORT_UCHAR((UINT)Address + INTERRUPT_ENABLE_REGISTER, 0);

    /* Enable DTR, RTS, and OUT2 */
    WRITE_PORT_UCHAR((UINT)Address + MODEM_CONTROL_REGISTER,
            SERIAL_MCR_DTR | SERIAL_MCR_RTS | SERIAL_MCR_OUT2);
    
    /* Set BaudRate */
    CpSetBaud(Port, BaudRate);

    /* Configure port parameters. */
    /* 
        Defaults:
            - 8 data bits
            - 1 stop bit
            - No parity
            - No break
    */
    WRITE_PORT_UCHAR((UINT)Port->Address + LINE_CONTROL_REGISTER,
            SERIAL_8_DATA | SERIAL_1_STOP | SERIAL_NONE_PARITY);
    
    /* Enable FIFO */
    CpEnableFifo(Address, TRUE);

    /* Clear RBR */
    READ_PORT_UCHAR((UINT)Address + RECEIVE_BUFFER_REGISTER);

    return STATUS_SUCCESS;
}


STATIC UCHAR RingIndicator;

STATIC
UCHAR
CpGetLineStatus(
    IN PCPPORT Port,
    IN UCHAR ExpectedValue
)
{
    UCHAR Lsr;
    Lsr = READ_PORT_UCHAR((UINT)Port->Address + LINE_STATUS_REGISTER);
    
    /* MSR stuff is todo */

    return Lsr;
}


USHORT
MINTAPI
CpGetByte(
    IN PCPPORT Port,
    OUT PUCHAR Byte,
    IN BOOLEAN Wait,
    IN BOOLEAN Poll
)
{
    return 0x0;
}

VOID
MINTAPI
CpPutByte(
    IN PCPPORT Port,
    IN UCHAR Byte
)
{
    /* Wait for transmit clear */
    while ((CpGetLineStatus(Port, SERIAL_LSR_THRE) & SERIAL_LSR_THRE) == 0x00);

    WRITE_PORT_UCHAR((UINT)Port->Address + TRANSMIT_HOLDING_REGISTER, Byte);
}