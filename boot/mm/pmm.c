/**
 * @file boot/mm/pmm.c
 * @brief Physical memory management of MINTLDR
 * 
 * @todo    This uses one of those naive PMM bitmaps. Could do much better with another style of allocator, 
 *          but this is literally a bootstub (not even a bootloader!)
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <mm/mm.h>
#include <ntdef.h>
#include <dbg.h>
#include <bugcheck.h>
#include <minilibc/string.h>

/* Physical memory map */
PMINTLDR_MEMORY_REGION PhysicalMemoryMap = NULL;

/* Frame bitmap + count */
PUINT32 PhysicalMemoryFrameBitmap = NULL;
SIZE_T PhysicalMemoryFrameCount = 0;
UINT_PTR PhysicalMemoryFirstFrameOffset = 0x0;

/* PMM statistics */
UINT32      PhysicalMemoryInUseBlocks = 0;
UINT32      PhysicalMemoryTotalBlocks = 0;

/* PMM free frame hints */
UINT32      PhysicalMemoryFirstFreeFrame = 0;

PMINTLDR_MEMORY_REGION MmGetPhysicalMemoryMap() {
    return PhysicalMemoryMap;
}

/* Frame setting/clearing/testing functions */
static VOID MmSetPhysicalFrame(UINT32 Frame) {
    PhysicalMemoryFrameBitmap[(Frame / 32)] |= ((unsigned)1 << (Frame % 32));
}

static VOID MmClearPhysicalFrame(UINT32 Frame) {
    PhysicalMemoryFrameBitmap[(Frame / 32)] &= ~((unsigned)1 << (Frame % 32));
}

static INT MmTestPhysicalFrame(UINT32 Frame) {
    return PhysicalMemoryFrameBitmap[(Frame / 32)] & ((unsigned)1 << (Frame % 32));
}


INT MmMarkPhysicalRegion(PMINTLDR_MEMORY_REGION Region) {
    PhysicalMemoryTotalBlocks += (Region->Size / MM_PAGE_SIZE);
    SIZE_T BlockBase = (SIZE_T)(Region->Base / MM_PAGE_SIZE);
    if (Region->MemoryType == RegionAvailable) {
        for (SIZE_T Block = 0; Block < (Region->Size / MM_PAGE_SIZE); Block++) {
            MmClearPhysicalFrame(BlockBase + Block);
        }
    } else {
        for (SIZE_T Block = 0; Block < (Region->Size / MM_PAGE_SIZE); Block++) {
            if (MmTestPhysicalFrame(BlockBase + Block)) continue;
            MmSetPhysicalFrame(BlockBase + Block);
        }

        PhysicalMemoryInUseBlocks += (Region->Size / MM_PAGE_SIZE);
    }

    return 0;
}

static UINT32 MmFindFirstFreePhysicalBlock() {
    UINT32 i = 0;
    for (; i < (PhysicalMemoryFrameCount / 32); i++) {
        if (PhysicalMemoryFrameBitmap[i] != 0xFFFFFFFF) {
            /* This block isn't full, find the first free bit */
            UINT32 Bit = 0;
            for (; Bit < 32; Bit++) {
                if (!(PhysicalMemoryFrameBitmap[i] & ((unsigned)1 << Bit))) {
                    return i * 32 + Bit;
                }
            }
        }
    }

    /* No free memory available */
    return 0xFFFFFFFF;
}

static UINT32 MmFindFirstFreePhysicalBlocks(SIZE_T BlockCount) {
    if (!BlockCount) return 0xFFFFFFFF;
    if (BlockCount == 1) return MmFindFirstFreePhysicalBlock();
    
    UINT32 i = 0;
    for (; i < (PhysicalMemoryFrameCount / 32); i++) {
        if (PhysicalMemoryFrameBitmap[i] != 0xFFFFFFFF) {
            /* The block isn't full, find the first free bit */
            UINT32 StartingBit = 0;
            for (; StartingBit < 32; StartingBit++) {
                if (!(PhysicalMemoryFrameBitmap[i] & ((unsigned)1 << StartingBit))) {
                    /* Right here */
                    UINT32 ReturnValue = i * 32 + StartingBit;

                    /* Verify enough frames are available */
                    UINT32 Block = 0;
                    INT FreeBlocks = 0;
                    for (; Block < BlockCount; Block++) {
                        if (MmTestPhysicalFrame(ReturnValue + Block)) {
                            break; /* This bit is taken, not contiguous enough */
                        }

                        FreeBlocks++;
                    }

                    if (FreeBlocks >= BlockCount) return ReturnValue;
                }
            }
        }
    }

    /* No free memory available */
    return 0xFFFFFFFF;
}


INT MmInitializeMemoryMap(PMINTLDR_MEMORY_REGION MapStart) {
    PhysicalMemoryMap = MapStart;
    
    /* First let's do some dumbassery. What we want is for our PMM bitmap to end on an available section. */
    /* The PMM bitmap will start at the first free region and end at the last free region */
    
    /* Get the bounds */
    PMINTLDR_MEMORY_REGION Region = MapStart;
    PMINTLDR_MEMORY_REGION FirstFreeRegion = NULL;
    PMINTLDR_MEMORY_REGION LastFreeRegion = NULL;
    while (Region) {
        if (Region->MemoryType == RegionAvailable) {
            LastFreeRegion = Region;

            if (!FirstFreeRegion) {
                FirstFreeRegion = Region;
            }
        }

        Region = Region->NextRegion;
    }

    if (!FirstFreeRegion) {
        MintBugCheckWithMessage(MEMORY_MAP_CORRUPTED, "No free regions found in memory map.\n");
    }

    /* We have valid bounds, let's figure how many frames that is */
    PhysicalMemoryFirstFrameOffset = FirstFreeRegion->Base;

    Region = FirstFreeRegion;
    while (Region && Region != LastFreeRegion->NextRegion) {
        DEBUG("Region is being used in PMM: %16x - %16x\n", Region->Base, Region->Base + Region->Size);
        PhysicalMemoryFrameCount += ((Region->Size / MM_PAGE_SIZE)); // Every byte can represent 8 frames
        Region = Region->NextRegion;
    }

    DEBUG("Found that %i frames are needed in physical memory\n", PhysicalMemoryFrameCount);

    /* Get space to put our bitmap */
    PhysicalMemoryFrameBitmap = (PUINT32)MmArchFindSpaceForPmmBitmap(PhysicalMemoryFrameCount / 8);
    if (!PhysicalMemoryFrameBitmap) {
        MintBugCheck(MEMORY_NO_SPACE);
    }

    /* Go over the regions one more time, marking them as free or not free */
    Region = FirstFreeRegion;
    while (Region && Region != LastFreeRegion->NextRegion) {
        MmMarkPhysicalRegion(Region);
        Region = Region->NextRegion;
    }
    
    /* Unmark the MINTLDR image */
    MmArchUnmarkMintldrImagePhysical();

    /* Unmark the PMM bitmap */
    MINTLDR_MEMORY_REGION PmmBitmapMemoryRegion = {
        .Base = (UINT_PTR)PhysicalMemoryFrameBitmap,
        .Size = (UINT_PTR)(PhysicalMemoryFrameCount / 8),
        .MemoryType = RegionLoaderHeap,
        .NextRegion = NULL,
        .PrevRegion = NULL
    };

    MmMarkPhysicalRegion(&PmmBitmapMemoryRegion);

    INFO("Physical memory manager is using %i blocks / %i blocks total\n", PhysicalMemoryInUseBlocks, PhysicalMemoryTotalBlocks);

    /* Should be good now */
    return 0;
}


UINT_PTR MmAllocatePhysicalPages(SIZE_T PageCount) {
    if (!PageCount) {
        WARN("Function 0x%x trying to allocate 0 pages - returning NULL\n", __builtin_return_address(0));
        return NULL;
    }

    /* Find the first few free frames */
    UINT32 Frame = MmFindFirstFreePhysicalBlocks(PageCount);
    if (Frame == 0xFFFFFFFF) {
        MintBugCheckWithMessage(OUT_OF_MEMORY, "Out of memory while trying to allocate %d bytes\n", PageCount * MM_PAGE_SIZE);
    }

    /* Set the frames */
    for (UINT32 i = 0; i < PageCount; i++) {
        MmSetPhysicalFrame(Frame + i);
    }

    PhysicalMemoryInUseBlocks += PageCount;

    DEBUG("MmAllocatePhysicalPages returning 0x%x\n", Frame*MM_PAGE_SIZE);
    return (UINT_PTR)(Frame * MM_PAGE_SIZE);
}

INT MmFreePhysicalPages(UINT_PTR Pages, SIZE_T PageCount) {
    if (!PageCount) {
        WARN("Function 0x%x tried to free 0 pages\n", __builtin_return_address(0));
        return 0;
    }

    /* Unset the frames */
    UINT32 Frame = ((UINT64)Pages / MM_PAGE_SIZE);

    for (UINT32 i = 0; i < PageCount; i++) {
        MmClearPhysicalFrame(Frame + i);
    }

    PhysicalMemoryInUseBlocks -= PageCount;

    return 0;
}