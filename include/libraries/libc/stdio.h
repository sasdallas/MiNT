/**
 * @file include/libraries/libc/stdio.h
 * @brief stdio
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef _STDIO_H
#define _STDIO_H

__MINT_BEGIN_DECLS

#include <stdarg.h>
#include <stddef.h>

int vsnprintf (char * s, size_t n, const char * format, va_list arg );

__MINT_END_DECLS

#endif