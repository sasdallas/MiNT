/**
 * @file mintkrnl/ki/arch/i386/ki_init.c
 * @brief MINTKRNL Ki initialization
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <arc/arc.h>
#include <mintdefs.h>
#include <WinDef.h>


DECLSPEC_IMPORT  UCHAR  KeGetCurrentIrql(VOID);


/* The big bang! */
CODE_SEG("INIT")
DECLSPEC_NORETURN
VOID
MINTAPI
KiSystemStartup(IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    __asm__ __volatile__("outb %b[Data], %w[Port]" :: [Port] "Nd" (0x3F8), [Data] "a" ('h'));
    for (;;);
}