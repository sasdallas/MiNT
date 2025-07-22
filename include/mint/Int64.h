/**
 * @file include/mint/Int64.h
 * @brief Fix to GCC
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_INT64_H__
#define __MINT_INT64_H__ 1

__MINT_BEGIN_DECLS

/* BAD! */
#define __int64 long long

__MINT_END_DECLS

#endif 