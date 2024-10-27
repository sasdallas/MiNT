/**
 * @file mint/include/mint/xdk/ntdef.h
 * @brief NT definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system
 * However, it has been released into the public domain and is free to use
 * This file comes with no warranty.
 */

#pragma once
#include <ddk/wdm.h>
#include <WinDef.h>


/* LIST_ENTRY structure */
typedef struct _LIST_ENTRY {
  struct _LIST_ENTRY *Flink;
  struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, PRLIST_ENTRY;


/* NTSTATUS */
typedef LONG NTSTATUS;

#define NT_SUCCESS(Status) ((Status >= 0 && Status <= 0x3FFFFFFF) || (Status >= 0x40000000 && Status <= 0x7FFFFFFF)) 
#define NT_INFORMATION(Status) (Status >= 0x40000000 && Status <= 0x7FFFFFFF)
#define NT_WARNING(Status) (Status >= 0x80000000 && Status <= 0xBFFFFFFF)
#define NT_ERROR(Status) (Status >= 0xC0000000 && Status <= 0xFFFFFFFF)

