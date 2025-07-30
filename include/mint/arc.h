/**
 * @file include/mint/arc.h
 * @brief Contains ARC stuff
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_ARC_H__
#define __MINT_ARC_H__ 1

#include <hardware/hardware.h>
#include <ntdef.h>

typedef struct _EFI_FIRMWARE_INFORMATION
{
     ULONG FirmwareVersion;
     PVOID VirtualEfiRuntimeServices;
     LONG SetVirtualAddressMapStatus;
     ULONG MissedMappingsCount;
} EFI_FIRMWARE_INFORMATION, *PEFI_FIRMWARE_INFORMATION;

typedef struct _FIRMWARE_INFORMATION_LOADER_BLOCK {
    struct {
        ULONG FirmwareTypeEfi : 1;
        ULONG EfiRuntimeUseIum : 1;
        ULONG EfiRuntimePageProtectionEnabled : 1;
        ULONG EfiRuntimePageProtectionSupported : 1;
        ULONG Reserved : 29;
    };

    union {
        EFI_FIRMWARE_INFORMATION EfiInformation;
    };
} FIRMWARE_INFORMATION_LOADER_BLOCK, *PFIRMWARE_INFORMATION_LOADER_BLOCK;


typedef struct _I386_LOADER_BLOCK {
    PVOID CommonDataArea;
    ULONG MachineType;
    ULONG VirtualBias;
} I386_LOADER_BLOCK, *PI386_LOADER_BLOCK;

typedef struct _LOADER_PARAMETER_BLOCK {
    ULONG OsMajorVersion;
    ULONG OsMinorVersion;
    ULONG Size;
    ULONG OsLoaderSecurityVersion;
    LIST_ENTRY LoadOrderListHead;
    LIST_ENTRY MemoryDescriptorListHead;
    LIST_ENTRY BootDriverListHead;
    LIST_ENTRY EarlyLaunchListHead;
    LIST_ENTRY CoreDriverListHead;
    LIST_ENTRY CoreExtensionsDriverListHead;
    LIST_ENTRY TpmCoreDriverListHead;
    ULONG_PTR KernelStack;
    ULONG_PTR Prcb;
    ULONG_PTR Process;
    ULONG_PTR Thread;
    ULONG KernelStackSize;
    ULONG RegistryLength;
    VOID *RegistryBase;
    VOID *ConfigurationRoot; /* CONFIGURATION_COMPONENT_DATA */
    PSTR ArcBootDeviceName;
    PSTR ArcHalDeviceName;
    PSTR NtBootPathName;
    PSTR NtHalPathName;
    PSTR LoadOptions;
    VOID *NlsData; /* NLS_DATA_BLOCK */
    VOID *ArcDiskInformation;  /* ARC_DISK_INFORMATION */
    PVOID OemFontFile;
    VOID *SetupLoaderBlock; /* SETUP_LOADER_BLOCK */
    VOID *Extension; /* LOADER_PARAMETER_EXTENSION */

    union {
        I386_LOADER_BLOCK I386;
    } u;

    FIRMWARE_INFORMATION_LOADER_BLOCK FirmwareInformation;
    PSTR OsBootstatPathName;
    PSTR ArcOSDataDeviceName;
    PSTR ArcWindowsSysPartName;
} LOADER_PARAMETER_BLOCK, *PLOADER_PARAMETER_BLOCK;


#endif