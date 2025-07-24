/**
 * @file boot/include/bugcheck.h
 * @brief Bugcheck header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_BUGCHECK_H__
#define __MINTLDR_BUGCHECK_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>
#include <winnt.h>

#define BOOT_INFORMATION_MISSING        ((ULONG)0x00000001)
#define MEMORY_NO_SPACE                 ((ULONG)0x00000002)
#define MEMORY_MAP_CORRUPTED            ((ULONG)0x00000003)
#define OUT_OF_MEMORY                   ((ULONG)0x00000004)
#define HEAP_CORRUPTION_DETECTED        ((ULONG)0x00000005)
#define MEMORY_REGION_LIST_CORRUPT      ((ULONG)0x00000006)

VOID MintBugCheck(UINT32 Bugcode);
VOID MintBugCheckWithMessage(UINT32 Bugcode, PCSTR Format, ...);

__MINT_END_DECLS

#endif