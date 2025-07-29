/**
 * @file include/mint/winnt.h
 * @brief WinNT header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_WINNT_H__
#define __MINT_WINNT_H__ 1

__MINT_BEGIN_DECLS

#define NTAPI __stdcall

#include <basetsd.h>
#include <ntdef.h>
#include <ddk/ntimage.h>
#include <sal.h>

#ifndef FORCEINLINE
#define FORCEINLINE     __attribute__((__always_inline__))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef C_ASSERT
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#endif

/* Type definitions */
typedef char            CHAR, *PCHAR;
typedef short           SHORT, *PSHORT;
typedef int             LONG, *PLONG;
typedef BOOL            BOOLEAN, *PBOOLEAN;

typedef VOID *PVOID;


__MINT_END_DECLS

#endif