/**
 * @file include/mint/ntdef.h
 * @brief NT definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_NTDEF_H__
#define __MINT_NTDEF_H__ 1

__MINT_BEGIN_DECLS

#include <basetsd.h>

#define CONST const

/* Uppercase types */
#ifndef VOID
typedef char CHAR;
typedef short SHORT;
typedef int LONG;
typedef int INT;
typedef double DOUBLE;
#define VOID void
#endif

/* Types */
typedef unsigned char UCHAR, *PUCHAR;
typedef unsigned short USHORT, *PUSHORT;
typedef unsigned int ULONG, *PULONG;
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *PCSTR;
typedef unsigned __int64 ULONGLONG, *PULONGLONG;

/* TODO: Should be in ntbasedef.h */
#define FIELD_OFFSET(Type, Field) ((LONG)__builtin_offsetof(Type, Field))

__MINT_END_DECLS

#endif