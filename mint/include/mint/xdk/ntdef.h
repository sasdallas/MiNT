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

#include <psdk/ntstatus.h>
#include <psdk/winerror.h>

/* LIST_ENTRY structure */
typedef struct _LIST_ENTRY {
  struct _LIST_ENTRY *Flink;
  struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, PRLIST_ENTRY;


