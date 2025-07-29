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

#include <Int64.h>
#include <msvctarget.h>

#if defined(_WIN64) 
 typedef __int64 INT_PTR; 
#else 
 typedef int INT_PTR;
#endif

#if defined(_WIN64)
 typedef __int64 LONG_PTR; 
#else
 typedef long LONG_PTR;
#endif

#if defined(_WIN64)
 typedef unsigned __int64 ULONG_PTR;
#else
 typedef unsigned long ULONG_PTR;
#endif

#if defined(_WIN64)
 typedef unsigned __int64 UINT_PTR;
#else
 typedef unsigned int UINT_PTR;
#endif

#ifdef _WIN64
 typedef unsigned int UHALF_PTR;
#else
 typedef unsigned short UHALF_PTR;
#endif

typedef unsigned long long DWORDLONG;
typedef unsigned long DWORD;
typedef unsigned long long DWORDLONG;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef WORD ATOM;
typedef WORD *PWORD;
typedef DWORD *PDWORD;
typedef int BOOL;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned __int64 UINT64;
typedef unsigned int ULONG32;
typedef unsigned __int64 ULONG64;

typedef unsigned __int64 ULONG64;
typedef ULONG_PTR DWORD_PTR;
typedef unsigned int DWORD32;
typedef unsigned __int64 DWORD64;
typedef signed char INT8;
typedef signed short INT16;
typedef signed int INT32;
typedef signed __int64 INT64;
typedef signed int LONG32;
typedef __int64 LONG64;
typedef DWORD_PTR *PDWORD_PTR;
typedef DWORD32 *PDWORD32;
typedef DWORD64 *PDWORD64;
typedef INT_PTR *PINT_PTR;
typedef INT8 *PINT8;
typedef INT16 *PINT16;
typedef INT32 *PINT32;
typedef INT64 *PINT64;
typedef LONG_PTR *PLONG_PTR;
typedef LONG32 *PLONG32;
typedef LONG64 *PLONG64;
#define POINTER_SIGNED __sptr
#define POINTER_UNSIGNED __uptr

typedef WORD *PWORD;

typedef ULONG_PTR SIZE_T;
typedef LONG_PTR SSIZE_T;
typedef SIZE_T *PSIZE_T;
typedef SSIZE_T *PSSIZE_T;
typedef UINT_PTR *PUINT_PTR;
typedef UINT8 *PUINT8;
typedef UINT16 *PUINT16;
typedef UINT32 *PUINT32;
typedef UINT64 *PUINT64;
typedef ULONG_PTR *PULONG_PTR;
typedef ULONG32 *PULONG32;
typedef ULONG64 *PULONG64;


static inline void *IntToPtr (const int val) { return ((void *)(INT_PTR)val); }
static inline void *UIntToPtr (const unsigned int val) { return ((void *)(UINT_PTR)val); }
static inline void *LongToPtr (const long val) { return ((void *)(LONG_PTR)val); }
static inline void *ULongToPtr (const unsigned long val) { return ((void *)(ULONG_PTR)val); }

#define NULL (0)

#define UlongToPtr ULongToPtr

__MINT_END_DECLS

#endif
