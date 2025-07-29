/**
 * @file include/mint/vadefs.h
 * @brief VA definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_VADEFS_H__
#define __MINT_VADEFS_H__ 1

__MINT_BEGIN_DECLS

/* va_list conversion */
#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
typedef __builtin_va_list va_list;
typedef va_list VA_LIST;
#endif

/* va functions */
#define va_start(v, l) __builtin_va_start(v, l)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_copy(d, s) __builtin_va_copy(d, s)

__MINT_END_DECLS

#endif
