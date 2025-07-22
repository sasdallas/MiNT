/**
 * @file boot/include/multiboot.h
 * @brief Multiboot header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_MULTIBOOT_H__
#define __MINTLDR_MULTIBOOT_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>
#include <winnt.h>

typedef struct MULTIBOOT_HEADER {
    UINT32          Flags;
    UINT32          MemLower;
    UINT32          MemUpper;
    UINT32          BootDevice;
    UINT32          CmdLine;

    UINT32          ModuleCount;
    UINT32          ModuleStart;

    UINT32          Symbols[4];

    UINT32          MemoryMapLength;
    UINT32          MemoryMapBase;

    UINT32          DrivesLength;
    UINT32          DrivesBase;

    UINT32          ConfigTable;
    UINT32          BootLoaderName;
    UINT32          ApmTable;

    UINT32          VbeControlInformation;
    UINT32          VbeModeInformation;
    UINT16          VbeMode;
    UINT16          VbeInterfaceSegment;
    UINT16          VbeInterfaceOffset;
    UINT16          VbeInterfaceLength;

    UINT64          FramebufferBase;
    UINT32          FramebufferPitch;
    UINT32          FramebufferWidth;
    UINT32          FramebufferHeight;
    UINT8           FramebufferBpp;
    UINT8           FramebufferType;
} __attribute__((packed)) MULTIBOOT_HEADER, *PMULTIBOOT_HEADER;

typedef struct MULTIBOOT_MODULE {
    UINT32          ModuleStart;
    UINT32          ModuleEnd;
    UINT32          ModuleCmdline;
    UINT32          ModulePadding;
} __attribute__((packed)) MULTIBOOT_MODULE, *PMULTIBOOT_MODULE;

typedef struct MULTIBOOT_MMAP_ENTRY {
    UINT32          Size;
    UINT64          Address;
    UINT64          Length;
    UINT32          Type;
} __attribute__((packed)) MULTIBOOT_MMAP_ENTRY, *PMULTIBOOT_MMAP_ENTRY;

#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

__MINT_END_DECLS

#endif