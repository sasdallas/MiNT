/**
 * @file boot/include/arch/x86_64/io.h
 * @brief I/O port library
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_ARCH_X86_64_IO_H__
#define __MINTLDR_ARCH_X86_64_IO_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>

VOID IoWrite8(UINT32 Port, UINT8 Value);
VOID IoWrite16(UINT32 Port, UINT16 Value);
VOID IoWrite32(UINT32 Port, UINT32 Value);

UINT8 IoRead8(UINT32 Port);
UINT16 IoRead16(UINT32 Port);
UINT32 IoRead32(UINT32 Port);

__MINT_END_DECLS

#endif