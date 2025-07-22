/**
 * @file boot/arch/x86_64/main.c
 * @brief MiNT loader main
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <winnt.h>
#include <ntdef.h>

void __stdcall MintLoaderMain() {
    *(PUINT8)0xb8000 = 'm';
    *(PUINT8)0xb8002 = 'i';
    for (;;);
}