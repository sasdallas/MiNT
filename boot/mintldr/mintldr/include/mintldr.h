/**
 * @file mintldr/include/mintldr.h
 * @brief MINTLDR Main Header
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef MINTLDR_H
#define MINTLDR_H

#include <WinDef.h>
#include <arch/i386/irq.h>
#include <ui/ui.h>
#include <dbg.h>


/*** MINTLDR Functions ***/

/* Bug check method */
VOID
__cdecl
MintBugCheckEx(
    _In_ ULONG BugCode,
    _In_ PCHAR File,
    _In_ ULONG Line
);

/* Bug check (but with text) */
void
__cdecl
MintBugCheckMsg(
    ULONG BugCode,
    PCHAR File,
    ULONG Line,
    PCSTR Format,
    ...
);

/* Bug code lists */
#define UNKNOWN_EXCEPTION 0
#define INSUFFICIENT_HARDWARE_REQUIREMENTS 1


#endif 