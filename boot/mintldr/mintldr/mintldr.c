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

void
__cdecl
MintStart() {
    /* Initialize mach system */
    MachInit();

    /* Initialize UI system */
    UiInit();


    UiPrint("MINTLDR v%s\n", VERSION_STR);
    UiPrint("Starting MiNT\n");

    UiPrint("\nUiPrint Testing\n");
    UiPrint("Integer: %i\n", 5);
    UiPrint("String: %s\n", "HELLO");
    UiPrint("Character: %c\n", 'g');
    UiPrint("Hexadecimal: 0x%x\n", 0xDEADBEEF);
    UiPrint("\tTabbing\n");


    WRITE_PORT_UCHAR(0x3F8, 'H');
    WRITE_PORT_UCHAR(0x3F8, 'i');
    WRITE_PORT_UCHAR(0x3F8, '!');
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

