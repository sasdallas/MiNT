/**
 * @file boot/include/arch/x86_64/mm.h
 * @brief Memory management definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_ARCH_X86_64_MM_H__
#define __MINTLDR_ARCH_X86_64_MM_H__ 1

__MINT_BEGIN_DECLS

#define MM_PAGE_SIZE            4096
#define MM_PAGE_SHIFT           12
#define MM_PAGE_MASK            0xFFF



__MINT_END_DECLS

#endif