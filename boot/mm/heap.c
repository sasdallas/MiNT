/**
 * @file boot/mm/heap.c
 * @brief MINTLDR heap implementation
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <mm/mm.h>
#include <winnt.h>
#include <dbg.h>
#include <bugcheck.h>

/* Default MINTLDR heap */
MINTLDR_HEAP __MintDefaultHeap = { 0 };
PMINTLDR_HEAP MintDefaultHeap = &__MintDefaultHeap;

INT MmInitHeap(PMINTLDR_HEAP Heap) {
    /* Nothing to do here, stub method */
    return 0;
}


PVOID MmAllocateHeap(PMINTLDR_HEAP Heap, SIZE_T Size) {
    if (!Heap->Base || !Heap->Size) {
        ERROR("Bad heap passed to MmAllocateHeap in attempt to get %i bytes: %16x\n", Size, Heap);
        return 0x0;
    }

    /* Actually needed size */
    Size = Size + sizeof(MINTLDR_HEAP_SMALL_BLOCK);

    /* Let's start going through each heap block */
    PMINTLDR_HEAP_BIG_BLOCK BigBlock = Heap->Head;
    while (BigBlock) {
        /* Does this block, supposedly, have enough? */
        if (BigBlock->BytesFree >= Size) {
            // DEBUG("Block %16x has enough space for us theoretically\n", BigBlock);

            /* First, check if any space is available at the start of the block */
            if ((UINT_PTR)BigBlock->Start - (UINT_PTR)BigBlock > Size) {
                DEBUG("Enough space at beginning\n");

                /* TODO */
                MintBugCheck(HEAP_CORRUPTION_DETECTED);
            }
            
            /* Not enough space between header -> first small block */
            /* Let's go searching for holes */
            PMINTLDR_HEAP_SMALL_BLOCK SmallBlock = BigBlock->Start;
            while (SmallBlock) {
                ASSERT_BUGCHECK(SmallBlock->Identifier == MINTLDR_HEAP_SMALL_BLOCK_ALIVE, HEAP_CORRUPTION_DETECTED, "Small block identifier tag corrupt");
                if (!SmallBlock->Next) break;
                
                /* Check the space between this and next */
                UINT_PTR Next = (UINT_PTR)SmallBlock->Next;
                UINT_PTR End = (UINT_PTR)SmallBlock + SmallBlock->Size + sizeof(MINTLDR_HEAP_SMALL_BLOCK);

                // DEBUG("Checking for space between %16x and %16x\n", End, Next);

                if (Next - End > Size) {
                    /* We have enough space, place a small block there */
                    PMINTLDR_HEAP_SMALL_BLOCK NewSmallBlock = (PMINTLDR_HEAP_SMALL_BLOCK)((UINT_PTR)SmallBlock + sizeof(MINTLDR_HEAP_SMALL_BLOCK) + SmallBlock->Size);
                    NewSmallBlock->Identifier = MINTLDR_HEAP_SMALL_BLOCK_ALIVE;
                    NewSmallBlock->Next = SmallBlock->Next;
                    NewSmallBlock->Prev = SmallBlock;
                    SmallBlock->Next = NewSmallBlock;
                    NewSmallBlock->Size = Size - sizeof(MINTLDR_HEAP_SMALL_BLOCK);
                    NewSmallBlock->Parent = BigBlock;
                    NewSmallBlock->Next->Prev = NewSmallBlock;
                    return ((PVOID)NewSmallBlock + sizeof(MINTLDR_HEAP_SMALL_BLOCK));
                }

                SmallBlock = SmallBlock->Next;
            }

            /* Check for enough space between the last small block and the end */
            if (SmallBlock) {
                UINT_PTR BigBlockEnd = (UINT_PTR)BigBlock + sizeof(MINTLDR_HEAP_BIG_BLOCK) + BigBlock->BytesTotal;
                UINT_PTR SmallBlockEnd = (UINT_PTR)SmallBlock + SmallBlock->Size + sizeof(MINTLDR_HEAP_SMALL_BLOCK);

                // DEBUG("Space check: %16x - %16x\n", SmallBlockEnd, BigBlockEnd);
                
                if (BigBlockEnd - SmallBlockEnd > Size) {
                    /* There's enough space in this block */
                    /* Create a new small block at the end of the previous small block */
                    PMINTLDR_HEAP_SMALL_BLOCK SmallBlockPrev = SmallBlock;

                    SmallBlock = (PMINTLDR_HEAP_SMALL_BLOCK)SmallBlockEnd;

                    SmallBlock->Identifier = MINTLDR_HEAP_SMALL_BLOCK_ALIVE;
                    SmallBlock->Size = Size - sizeof(MINTLDR_HEAP_SMALL_BLOCK);
                    SmallBlock->Prev = SmallBlockPrev;
                    SmallBlockPrev->Next = SmallBlock;
                    SmallBlock->Next = NULL;
                    SmallBlock->Parent = BigBlock;
                    
                    return (PVOID)SmallBlock + sizeof(MINTLDR_HEAP_SMALL_BLOCK);
                }
            } else {
                /* The heap has no head */
                /* We can just make a new small block at the start */
                /* TODO */
                MintBugCheck(HEAP_CORRUPTION_DETECTED);
            }
        }

        if (!BigBlock->Next) break;
        BigBlock = BigBlock->Next;
    }

    /* Nothing is available in this heap. Can we get another big block? */
    PMINTLDR_HEAP_BIG_BLOCK BigBlockPrevious = BigBlock;
    if (!Heap->Head) {
        BigBlock = (PMINTLDR_HEAP_BIG_BLOCK)Heap->Base;
    } else {
        BigBlock = (PMINTLDR_HEAP_BIG_BLOCK)((UINT_PTR)BigBlock + BigBlock->BytesTotal + sizeof(MINTLDR_HEAP_BIG_BLOCK));
    }

    /* Needed size */
    SIZE_T NeededSize = MM_PAGE_ALIGN_UP(Size + sizeof(MINTLDR_HEAP_BIG_BLOCK));

    /* Can we allocate this block? */
    if ((UINT_PTR)BigBlock + NeededSize >= Heap->Base + Heap->Size) {
        /* TODO: Heap expansion */
        MintBugCheckWithMessage(OUT_OF_MEMORY, "Out of memory in MmAllocateHeap for %d bytes\n", NeededSize);
    }

    /* Yeah, we can. Go get it! */
    for (UINT_PTR i = (UINT_PTR)BigBlock; i < (UINT_PTR)BigBlock + NeededSize; i += MM_PAGE_SIZE) {
        MINTLDR_PAGE Page = {
            .Flags = MINTLDR_PAGE_PRESENT | MINTLDR_PAGE_WRITABLE,
            .Address = MmAllocatePhysicalPages(1),
        };

        MmArchSetPage(&Page, i);
    }

    DEBUG("New big block at %16x - %16x created\n", BigBlock, (UINT_PTR)BigBlock + NeededSize);


    BigBlock->Next = NULL;
    BigBlock->Prev = NULL;

    if (!BigBlockPrevious) {
        Heap->Head = BigBlock;
    } else {
        BigBlock->Prev = BigBlockPrevious;
        BigBlockPrevious->Next = BigBlock;
    }

    BigBlock->BytesTotal = NeededSize - sizeof(MINTLDR_HEAP_BIG_BLOCK);
    BigBlock->BytesFree = BigBlock->BytesTotal - Size;
    BigBlock->Identifier = MINTLDR_HEAP_BIG_BLOCK_TAG;
    BigBlock->Start = (PMINTLDR_HEAP_SMALL_BLOCK)((UINT_PTR)BigBlock + sizeof(MINTLDR_HEAP_BIG_BLOCK));

    BigBlock->Start->Identifier = MINTLDR_HEAP_SMALL_BLOCK_ALIVE;
    BigBlock->Start->Next = NULL;
    BigBlock->Start->Prev = NULL;
    BigBlock->Start->Parent = BigBlock;
    BigBlock->Start->Size = Size - sizeof(MINTLDR_HEAP_SMALL_BLOCK);

    return (PVOID)BigBlock->Start + sizeof(MINTLDR_HEAP_SMALL_BLOCK);
}


INT MmInitDefaultHeap() {
    MmInitHeap(MintDefaultHeap);

    PMINTLDR_MEMORY_REGION Region;
    MintDefaultHeap->Base = MmAllocatePagesEx(32, &Region);
    MintDefaultHeap->Size = 32 * MM_PAGE_SIZE;
    MintDefaultHeap->Head = NULL;
    MintDefaultHeap->Type = RegionLoaderHeap;
    Region->MemoryType = RegionLoaderHeap;
    
    return 0;
}

// PMINTLDR_HEAP MmCreateHeapEx(MINTLDR_MEMORY_TYPE MemoryType, UINT_PTR Base, SIZE_T Size) {
//     return NULL; /* TODO */
// }

// PMINTLDR_HEAP MmCreateHeap(MINTLDR_MEMORY_TYPE MemoryType) {
//     return MmCreateHeapEx(MemoryType, MmAllocatePages(32), 32 * MM_PAGE_SIZE);
// }

// INT MmFreeHeap(PMINTLDR_HEAP Heap, UINT_PTR Allocation) {
//     /* TODO */
//     DEBUG("MmFreeHeap %016x\n", Allocation);
//     return 0;
// }

// INT MmDestroyHeap(PMINTLDR_HEAP Heap) {
//     /* TODO */
//     DEBUG("MmDestroyHeap %016x\n", Heap);
//     return 0;
// }