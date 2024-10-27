/**
 * @file mintldr/arch/i386/mach/memory.c
 * @brief Memory functions
 * 
 * Provides all memory functions for I386 architecture.
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */


#include <mintldr.h>
#include <arch/i386/mach.h>
#include <arch/i386/i386.h>
#include <dbg.h>

STATIC BIOS_MEMORY_MAP MachMemoryMap[81];


/* Get memory map */
STATIC
INT
MachGetMemoryMap(
    IN PBIOS_MEMORY_MAP *MemoryMap,
    UINT MaxMemoryDescriptors
)
{
    REGISTERS Registers;
    INT MapCount = 0;

    INFO(("Getting BIOS memory map..."));
    
    /* Set the incrementation counter to 0 */
    Registers.x.ebx = 0x00000000;

    while (MapCount < MaxMemoryDescriptors) 
    {
        /* Setup registers for the call */
        Registers.x.eax = 0x0000E820;
        Registers.x.edx = 0x534D4150;
        Registers.x.ecx = sizeof(BIOS_MEMORY_MAP);

        Registers.w.es = BIOSCALLBUFSEGMENT;
        Registers.w.di = BIOSCALLBUFOFFSET;

        /* Call BIOS */
        bios32(0x15, &Registers, &Registers);

        /* Check if BIOS returned 0x534D4150 in EAX */
        if (Registers.x.eax != 0x534D4150)
        {
            MintBugCheckMsg(INSUFFICIENT_HARDWARE_REQUIREMENTS,
                                __FILE__, __LINE__,
                                (PCSTR)"Your system does not support memory maps with the E820 method.\n");
        }


        /* CF was set, we're done */
        if (!BIOS32_SUCCESS(Registers))
        {
            INFO(("Finished reading memory maps"));
            break;
        }

        /* Display information */
        PBIOS_MEMORY_MAP MemoryMapEntry = (PBIOS_MEMORY_MAP)BIOSCALLBUFFER;
        INFO(("MapCount %i\n\tBaseAddress: 0x%x", MapCount, MemoryMapEntry->ba.BaseAddressLow));
        
        /* Increment MapCount */
        MapCount++;

        /* Check if EBX was 0 */
        if (Registers.x.ebx == 0)
        {
            INFO(("Finished reading memory map"));
            break;
        } 
    }
}


/* Initialize memory systems */
VOID
MachInitializeMemory()
{
    INFO(("Starting memory subsystem..."));

    /* Get the memory map */   
    INT Entries = MachGetMemoryMap(MachMemoryMap, 81);
    UiPrint("MACH: Initialized memory subsystem\n");
}