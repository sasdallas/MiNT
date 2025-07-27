/**
 * @file include/mint/kddll.h
 * @brief Kernel debugger DLL definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_KDDLL_H__
#define __MINT_KDDLL_H__ 1

__MINT_BEGIN_DECLS

#include <winnt.h>
#include <ntstatus.h>

NTAPI
NTSTATUS
KdpDebugPrint(
    PCHAR Format,
    ...
);

NTAPI
NTSTATUS
KdDebuggerInitialize0(
    IN OPTIONAL PVOID LoaderParamterBlock
);

__MINT_END_DECLS

#endif 