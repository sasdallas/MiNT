/**
 * @file boot/include/ldr/ldr.h
 * @brief PE loader
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_LDR_LDR_H__
#define __MINTLDR_LDR_LDR_H__ 1

__MINT_BEGIN_DECLS

#include <winnt.h>
#include <mm/region.h>

INT LdrImageLoad(UINT_PTR Base, MINTLDR_MEMORY_TYPE MemoryType, PVOID *ImageBase);

__MINT_END_DECLS

#endif 