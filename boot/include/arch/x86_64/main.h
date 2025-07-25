/**
 * @file boot/include/arch/x86_64/main.h
 * @brief Main/important types
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_ARCH_X86_64_MAIN_H__
#define __MINT_ARCH_X86_64_MAIN_H__ 1

__MINT_BEGIN_DECLS

#include <winnt.h>

/**
 * MINTLDR, when using Multiboot, loads MINTKRNL and drivers from Multiboot modules
 * 
 * Because this project is "NT-like" and not a *FULL* NT recreation, I can do whatever the hell I want.
 * The core kernel drivers are loaded from a USTAR archive of a ramdisk, and the kernel will load the rest into memory
 */
typedef enum {
    ModuleUnknown,
    ModuleKernel,
} MINTLDR_MODULE_TYPE;

typedef struct MINTLDR_MODULE {
    MINTLDR_MODULE_TYPE     Type;               // Type of module
    UINT_PTR                PhysicalPageBase;   // Physical page base
    UINT_PTR                Base;               // Module base in memory
    SIZE_T                  Size;               // Module size
} MINTLDR_MODULE, *PMINTLDR_MODULE; 

extern MINTLDR_MODULE ModuleList[];
extern SIZE_T ModuleCount;

__MINT_END_DECLS

#endif