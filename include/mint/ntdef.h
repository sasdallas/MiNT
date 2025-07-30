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
#include <minwindef.h>

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

#include <ntbasedef.h>
#include <ntdef_types.h>

typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, PRLIST_ENTRY;

__MINT_END_DECLS

#endif