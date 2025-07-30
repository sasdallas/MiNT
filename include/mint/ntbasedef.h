/**
 * @file include/mint/ntbasedef.h
 * @brief NT base definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_NTBASEDEF_H__
#define __MINT_NTBASEDEF_H__

#ifdef __INTELLISENSE__
#include <ntdef.h>
#endif

#include <sal.h>

#ifndef DUMMYUNIONNAME
    #define DUMMYUNIONNAME
#endif

#ifndef DECLSPEC_EXPORT
#define DECLSPEC_EXPORT __declspec(dllexport)
#endif

#ifndef DECLSPEC_IMPORT
#define DECLSPEC_IMPORT __declspec(dllimport)
#endif

#ifndef DECLSPEC_NORETURN
#define DECLSPEC_NORETURN __declspec(noreturn)
#endif

#ifndef DECLSPEC_ALIGN
    #if defined(__GNUC__)
        #define DECLSPEC_ALIGN(x) __attribute__((aligned(x)))
    #endif
#endif

typedef unsigned char UCHAR, *PUCHAR;
typedef unsigned short USHORT, *PUSHORT;
typedef unsigned int ULONG, *PULONG;
typedef unsigned __int64 ULONGLONG, *PULONGLONG;
typedef __int64 LONGLONG, *PLONGLONG;

typedef CHAR CCHAR;
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCSTR, *PCSTR;
typedef _Null_terminated_ CHAR *LPSTR, *PSTR;

typedef unsigned int UINT, *PUINT;

typedef VOID *PVOID;


#ifndef FIELD_OFFSET
    #if defined(__GNUC__)
        #define FIELD_OFFSET(Type, Field) ((LONG)__builtin_offsetof(Type, Field))
    #endif
#endif

#endif