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

    REGISTERS Registers;
    Registers.x.eflags = 0;
    Registers.b.ah = 0x04;
    bios32(0x1A, &Registers, &Registers);

    if (!BIOS32_SUCCESS(Registers)) UiPrint("Failed\n");
    else UiPrint("OK");
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

