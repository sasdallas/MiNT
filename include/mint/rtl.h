/**
 * @file include/mint/rtl.h
 * @brief RTL functions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_RTL_H__
#define __MINT_RTL_H__ 1

#include <basetsd.h>
#include <winnt.h>

VOID
NTAPI
RtlFillMemory(
    PVOID Destination,
    SIZE_T Length,
    UCHAR Fill
);

VOID
NTAPI
RtlZeroMemory(
    void*  Destination,
    SIZE_T Length
);

VOID
NTAPI
RtlMoveMemory(
    PVOID Destination,
    CONST PVOID Source,
    SIZE_T Length
);

#endif