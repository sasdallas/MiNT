/**
 * @file include/mint/msvctarget.h
 * @brief GCC target -> MSVC target
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_MSVCTARGET_H__
#define __MINT_MSVCTARGET_H__ 1

__MINT_BEGIN_DECLS

#if defined(__i386__)
#define _X86_ 1
#define _M_IX86 1
#elif defined(__x86_64__)
#define _AMD64_     1
#define _M_AMD64    1
#define _M_X64      1
#else
#error "Unknown arch"
#endif

__MINT_END_DECLS

#endif