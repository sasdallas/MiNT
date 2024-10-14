// BaseTsd.h - Basic Windows datatype library
// This file is not protected and is released to the public domain
#pragma once

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

#include <Int64.h>
#include <WinDef.h>


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

