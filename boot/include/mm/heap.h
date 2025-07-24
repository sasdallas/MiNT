/**
 * @file boot/include/mm/heap.h
 * @brief MINTLDR heap
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_MM_HEAP_H__
#define __MINT_MM_HEAP_H__ 1

__MINT_BEGIN_DECLS

#include <winnt.h>
#include <mm/region.h>

#define MINTLDR_HEAP_BIG_BLOCK_TAG              0xDEADBEEF
#define MINTLDR_HEAP_SMALL_BLOCK_ALIVE          0x13371337
#define MINTLDR_HEAP_SMALL_BLOCK_DEAD           0xDEADDEAD

struct MINTLDR_HEAP_BIG_BLOCK;

/* MINTLDR heap small block */
typedef struct MINTLDR_HEAP_SMALL_BLOCK {
    UINT32                              Identifier;     /* Identifier */
    SIZE_T                              Size;           /* Size, in bytes, of this small block */
    struct MINTLDR_HEAP_SMALL_BLOCK*    Next;           /* Next */
    struct MINTLDR_HEAP_SMALL_BLOCK*    Prev;           /* Previous */
    struct MINTLDR_HEAP_BIG_BLOCK*      Parent;         /* Parent */
} MINTLDR_HEAP_SMALL_BLOCK, *PMINTLDR_HEAP_SMALL_BLOCK;

/* MINTLDR heap big block */
typedef struct MINTLDR_HEAP_BIG_BLOCK {
    UINT32                              Identifier;     /* Identifier of this big block */
    SIZE_T                              BytesTotal;     /* Total bytes in this big block */
    SIZE_T                              BytesFree;      /* Bytes free (out of BytesTotal) in this big block */
    struct MINTLDR_HEAP_BIG_BLOCK*      Next;           /* Next big block in the chain */
    struct MINTLDR_HEAP_BIG_BLOCK*      Prev;           /* Previous big block in the chain */
    PMINTLDR_HEAP_SMALL_BLOCK           Start;          /* Starting small block */
} MINTLDR_HEAP_BIG_BLOCK, *PMINTLDR_HEAP_BIG_BLOCK;

/* MINTLDR heap */
typedef struct MINTLDR_HEAP {
    MINTLDR_MEMORY_TYPE         Type;       /* Type of memory */
    UINT_PTR                    Base;       /* Heap base */
    SIZE_T                      Size;       /* Heap size */
    PMINTLDR_HEAP_BIG_BLOCK     Head;       /* First big block of the head */
} MINTLDR_HEAP, *PMINTLDR_HEAP;

extern PMINTLDR_HEAP MintDefaultHeap;

INT             MmInitHeap(PMINTLDR_HEAP Heap);
INT             MmInitDefaultHeap();
PMINTLDR_HEAP   MmCreateHeapEx(MINTLDR_MEMORY_TYPE MemoryType, UINT_PTR Base, SIZE_T Size);
PMINTLDR_HEAP   MmCreateHeap(MINTLDR_MEMORY_TYPE MemoryType);
PVOID           MmAllocateHeap(PMINTLDR_HEAP Heap, SIZE_T Size);
INT             MmFreeHeap(PMINTLDR_HEAP Heap, UINT_PTR Allocation);
INT             MmDestroyHeap(PMINTLDR_HEAP Heap);

__MINT_END_DECLS

#endif