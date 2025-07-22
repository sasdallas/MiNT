/**
 * @file include/mint/ntstatus.h
 * @brief NT status
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_NTSTATUS_H__
#define __MINT_NTSTATUS_H__

__MINT_BEGIN_DECLS

#include <winnt.h>

#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif

#define STATUS_SUCCESS          ((NTSTATUS)0x00000000)

__MINT_END_DECLS

#endif