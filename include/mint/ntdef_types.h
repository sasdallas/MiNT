/**
 * @file include/mint/ntdef_types.h
 * @brief Structures in ntdef.h
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_NTDEF_TYPES_H__
#define __MINT_NTDEF_TYPES_H__ 1 

__MINT_BEGIN_DECLS

/* lol */
#ifdef __INTELLISENSE__
#include <ntdef.h>
#endif

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING, *PSTRING;

__MINT_END_DECLS

#endif