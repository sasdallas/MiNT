/**
 * @file boot/include/arch/serial.h
 * @brief Architecture serial port handler
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_ARCH_SERIAL_H__
#define __MINTLDR_ARCH_SERIAL_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>

INT SerialInitialize(ULONG ComPort, ULONG BaudRate);
VOID SerialPrintCharacter(CHAR Character);
VOID SerialPrint(PCHAR Format, ...);

__MINT_END_DECLS

#endif