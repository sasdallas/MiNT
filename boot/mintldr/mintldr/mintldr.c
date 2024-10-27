/**
 * @file mintldr/mintldr.c
 * @brief Main MINTLDR file
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <mintldr.h>
#include <gfx.h>
#include <vadefs.h>
#include <ui/ui.h>
#include <arch/mach.h>
#include <arch/i386/i386.h>
#include <arch/i386/mach.h>

#include <asm/reactos/x86common.h>

/* BIOS Memory Map */
typedef struct
{
    // ACPI 1.0.
    ULONGLONG   BaseAddress;
    ULONGLONG   Length;
    ULONG       Type;
    // ACPI 3.0.
    union
    {
        ULONG   ExtendedAttributesAsULONG;

        struct
        {
            // Bit 0. ACPI 3.0.
            // As of ACPI 4.0, became "Reserved -> must be 1".
            ULONG Enabled_Reserved : 1;
            // Bit 1. ACPI 3.0.
            // As of ACPI 6.1, became "Unimplemented -> Deprecated".
            // As of ACPI 6.3, became "Reserved -> must be 0".
            ULONG NonVolatile_Deprecated_Reserved : 1;
            // Bit 2. ACPI 4.0.
            // As of ACPI 6.1, became "Unimplemented -> Deprecated".
            // As of ACPI 6.3, became "Reserved -> must be 0".
            ULONG SlowAccess_Deprecated_Reserved : 1;
            // Bit 3. ACPI 4.0.
            // ACPI 5.0-A added "Used only on PC-AT BIOS" (not UEFI).
            ULONG ErrorLog : 1;
            // Bits 4-31. ACPI 3.0.
            ULONG Reserved : 28;
        } ExtendedAttributes;
    };
} BIOS_MEMORY_MAP, *PBIOS_MEMORY_MAP;

void
__cdecl
MintStart() {
    /* Initialize UI system */
    UiInit();

    /* Initialize mach system */
    MachInit();

    /* Write basic information */
    INFO(("MINTLDR v%s", VERSION_STR));
    

    UiPrint("MINTLDR v%s\n", VERSION_STR);
    UiPrint("Starting MiNT\n");

    UiPrint("MINT: Performing register call\n");
    REGISTERS Registers;
    Registers.x.ebx = 0x00000000;
    Registers.x.eax = 0x0000E820;
    Registers.x.edx = 0x534D4150;
    Registers.x.ecx = sizeof(BIOS_MEMORY_MAP);
    Registers.w.es = BIOSCALLBUFSEGMENT;
    Registers.w.di = BIOSCALLBUFOFFSET;
    bios32(0x15, &Registers, &Registers);

    UiPrint("MINT: Register call completed. Results:\n");
    UiPrint("EAX=0x%x EBX = 0x%x ECX = 0x%x EDX = 0x%x\n", Registers.x.eax, Registers.x.ebx, Registers.x.ecx, Registers.x.edx);
 
    for (;;);
}


/* TODO: Move this? */
PCHAR BugCodeMessages[] =
{
    "UNKNOWN_EXCEPTION"
};

/* Bug check method */
void
__cdecl
MintBugCheckEx(
    ULONG BugCode,
    PCHAR File,
    ULONG Line
)
{
    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiClearScreen(); 

    GfxDrawPopup(0, 0, VGA_WIDTH, VGA_HEIGHT, "MINTLDR - Fatal Error", "");

    // GfxDrawPopup will be replaced by UiCreatePopup eventually, but for now write over the text
    UiSetColors(COLOR_BLACK, COLOR_LIGHT_GRAY);
    UiSetPosition(0, 0);
    UiPrint("MINTLDR v%s - Fatal Error", VERSION_STR);

    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiSetPosition(0, 2);
    
    UiPrint("MINTLDR encountered a fatal exception while loading.\n");

    UiPrint("*** STOP 0x%8x: %s\n", BugCode, BugCodeMessages[BugCode]);

    if (File)
    {
        UiPrint("*** Exception occurred at %s:%i\n", File, Line);
    }


    UiPrint("\nReport this error to the MiNT GitHub repository.\n");
    UiPrint("\nSystem halted.\n");

    asm volatile ("cli");
    asm volatile ("hlt");

    for (;;);
}

/* Bug check (but with text) */
void
__cdecl
MintBugCheckMsg(
    ULONG BugCode,
    PCHAR File,
    ULONG Line,
    PCSTR *Format,
    ...
)
{

    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiClearScreen(); 

    GfxDrawPopup(0, 0, VGA_WIDTH, VGA_HEIGHT, "MINTLDR - Fatal Error", "");

    // GfxDrawPopup will be replaced by UiCreatePopup eventually, but for now write over the text
    UiSetColors(COLOR_BLACK, COLOR_LIGHT_GRAY);
    UiSetPosition(0, 0);
    UiPrint("MINTLDR v%s - Fatal Error", VERSION_STR);

    UiSetColors(COLOR_WHITE, COLOR_BLUE);
    UiSetPosition(0, 2);
    
    UiPrint("MINTLDR encountered a fatal exception while loading.\n");
    
    /* BugCode is optional */
    if (BugCode)
    {
        UiPrint("*** STOP 0x%8x: %s\n", BugCode, BugCodeMessages[BugCode]);
    }

    if (File)
    {
        UiPrint("*** Exception at %s:%i\n", File, Line);
    }

    UiPrint("\n");

    /* Print out their messages */
    va_list ap;
    va_start(ap, Format);
    UiVAPrint(Format, ap);
    va_end(ap);

    /* Sign off & halt */
    UiPrint("\nReport this error to the MiNT GitHub repository.\n");
    UiPrint("\nSystem halted.\n");

    asm volatile ("cli");
    asm volatile ("hlt");

    for (;;);
}

