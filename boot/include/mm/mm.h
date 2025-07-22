/**
 * @file boot/include/mm/mm.h
 * @brief MINTLDR memory management subsystem
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_MM_MM_H__
#define __MINT_MM_MM_H__ 1

__MINT_BEGIN_DECLS

#include <mm/region.h>
#include <msvctarget.h>
#include <winnt.h>

#ifdef _M_AMD64
#include <arch/x86_64/mm.h>
#else
#error "Unknown architecture"
#endif

INT MmInitializeMemoryMap(PMINTLDR_MEMORY_REGION MapStart);

__MINT_END_DECLS

#endif