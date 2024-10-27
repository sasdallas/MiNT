/**
 * @file mint/include/mint/arc/arc.h
 * @brief ARC definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system
 * However, it has been released into the public domain and is free to use
 * This file comes with no warranty.
 */


#pragma once

#include <xdk/ntdef.h>
#include <WinDef.h>

/**
 * I386_LOADER_BLOCK
 */
typedef struct _I386_LOADER_BLOCK
{
    PVOID CommonDataArea;
    ULONG MachineType;
    ULONG VirtualBias;
} I386_LOADER_BLOCK, *PI386_LOADER_BLOCK;

/**
 * FIRMWARE_INFORMATION_LOADER_BLOCK
 */
typedef struct _FIRMWARE_INFORMATION_LOADER_BLOCK
{
    ULONG FirmwareTypeEfi:1;
    ULONG Reserved:31;
    /* EFI boots should contain some information here. Or you could go PCAT. But that's not needed. */
} FIRMWARE_INFORMATION_LOADER_BLOCK, *PFIRMWARE_INFORMATION_LOADER_BLOCK;

/**
 * LOADER_PARAMETER_BLOCK
 * 
 * Kernel loader parameter block. See http://www.geoffchappell.com/studies/windows/km/ntoskrnl/structs/loader_parameter_block.htm
 * for more details.
 */
typedef struct _LOADER_PARAMETER_BLOCK
{
    LIST_ENTRY LoadOrderListHead;
    LIST_ENTRY MemoryDescriptorListHead;
    LIST_ENTRY BootDriverListHead;
    ULONG_PTR KernelStack;
    ULONG_PTR Prcb;
    ULONG_PTR Process;
    ULONG_PTR Thread;
    PVOID RegistryBase;
    PVOID ConfigurationRoot; /* PCONFIGURATION_COMPONENT_DATA */
    PSTR ArcBootDeviceName;
    PSTR ArcHalDeviceName;
    PSTR NtBootPathName;
    PSTR NtHalPathName;
    PSTR LoadOptions;
    PVOID NlsData; /* PNLS_DATA_BLOCK */
    PVOID ArcDiskInformation; /* PARC_DISK_INFORMATION */
    PVOID OemFontFile;
    PVOID SetupLdrBlock; /* struct _SETUP_LOADER_BLOCK */
    PVOID Extension; /* LOADER_PARAMETER_EXTENSION */

    /* Architecture-specific so part of a union */
    union {
        I386_LOADER_BLOCK I386;
    } arch;

    FIRMWARE_INFORMATION_LOADER_BLOCK FirmwareInformation;
} LOADER_PARAMETER_BLOCK, *PLOADER_PARAMETER_BLOCK;