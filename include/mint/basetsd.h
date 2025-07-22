/**
 * @file include/mint/basetsd.h
 * @brief Base types
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_BASETSD_H__
#define __MINT_BASETSD_H__ 1

__MINT_BEGIN_DECLS

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef signed int          LONG32, *PLONG32;
typedef unsigned int        ULONG32, *PULONG32;
typedef unsigned int        DWORD32, *PDWORD32;

typedef signed long long          INT64;
typedef unsigned long long        UINT64;
typedef signed long long          LONG64;
typedef unsigned long long        ULONG64;
typedef unsigned long long        DWORD64;

__MINT_END_DECLS

#endif
