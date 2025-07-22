/**
 * @file boot/include/minilibc/stdlib.h
 * @brief Standard library
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_MINILIBC_STDLIB_H__
#define __MINTLDR_MINILIBC_STDLIB_H__ 1

__MINT_BEGIN_DECLS

long int strtol(const char *str, char **endptr, int base);

__MINT_END_DECLS

#endif