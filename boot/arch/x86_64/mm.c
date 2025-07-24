/**
 * @file boot/arch/x86_64/mm.c
 * @brief x86_64 memory management subsystem
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <arch/x86_64/mm.h>
#include <mm/mm.h>
#include <dbg.h>
#include <ntdef.h>
#include <minilibc/string.h>

extern UINT32 __mintldr_image_start;
extern UINT32 __mintldr_image_end;

/* Base PML, contains enough space to hold some initial PDPTs/PDs/PTs as well */
MEMORY_PAGE MintLoaderBasePML[3][512] __attribute__((aligned(MM_PAGE_SIZE))) = { 0 };

/* PDPT/PD/PT that contains the lower physical memory mapping for MINTLDR */
MEMORY_PAGE MintLoaderLowMapPDPT[512] __attribute__((aligned(MM_PAGE_SIZE)))            = { 0 };
MEMORY_PAGE MintLoaderLowMapPD[512] __attribute__((aligned(MM_PAGE_SIZE)))              = { 0 };
MEMORY_PAGE MintLoaderLowMapPT[512*2] __attribute__((aligned(MM_PAGE_SIZE)))              = { 0 };

/* PDPT/PD/PT that contains the physical memory identity mapping */
MEMORY_PAGE MintLoaderIdentityMapPDPT[512] __attribute__((aligned(MM_PAGE_SIZE)))       = { 0 };
MEMORY_PAGE MintLoaderIdentityMapPD[128][512] __attribute__((aligned(MM_PAGE_SIZE)))    = { 0 };

UINT_PTR MmArchFindSpaceForPmmBitmap(SIZE_T SpaceRequired) {
    /* For now place the PMM buffer at the end. This isn't how you memory manage */
    DEBUG("MINTLDR image bounds: 0x%x - 0x%x\n", MM_PAGE_ALIGN_DOWN((UINT64)&__mintldr_image_start), MM_PAGE_ALIGN_UP(((UINT64)&__mintldr_image_end)));
    
    /* TODO: Validate this spot is available */
    UINT_PTR PmmBufferLocation = (UINT_PTR)MM_PAGE_ALIGN_UP((UINT64)&__mintldr_image_end);
    DEBUG("Placing PMM buffer at 0x%x\n", PmmBufferLocation);
    return PmmBufferLocation;
}

UINT_PTR MmArchFindSpaceForInitialHeap(SIZE_T SpaceRequired) {
    /* TODO: Validate this spot is available */
    UINT_PTR InitialHeapLocation = (UINT_PTR)MM_PAGE_ALIGN_UP((UINT64)&__mintldr_image_end);
    DEBUG("Placing initial heap at 0x%x\n", InitialHeapLocation);
    return InitialHeapLocation;
}

VOID MmArchUnmarkMintldrImagePhysical() {
    UINT_PTR MintldrImageStart, MintldrImageSize;
    MintldrImageStart = (UINT_PTR)MM_PAGE_ALIGN_DOWN((UINT64)&__mintldr_image_start);
    MintldrImageSize = (UINT_PTR)MM_PAGE_ALIGN_UP((UINT64)&__mintldr_image_end) - MintldrImageStart;

    /* Unmark the Mintldr image data */
    MINTLDR_MEMORY_REGION Region = {
        .Base = (UINT_PTR)MintldrImageStart,
        .Size = (UINT_PTR)MintldrImageSize,
        .MemoryType = RegionLoaderCode,
        .NextRegion = NULL,
        .PrevRegion = NULL,
    };

    MmMarkPhysicalRegion(&Region);
}

VOID MmArchUnmarkMintldrImage() {

}

INT MmArchSetupPageMap() {
    /* The bootloader setup an initial stub for us with valid addresses in the low memory, but we have to setup identity map */

    /* We should probably also build a new low memory map not using 2MB pages */
    /* Two PTs should be all we need */
    /* TODO: Implement checks */

    MintLoaderLowMapPDPT[0].Present = 1;
    MintLoaderLowMapPDPT[0].Writable = 1;
    MintLoaderLowMapPDPT[0].Address = ((UINT64)&(MintLoaderLowMapPD)) >> MM_PAGE_SHIFT;

    for (SIZE_T i = 0; i < 2; i++) {
        MintLoaderLowMapPD[i].Present = 1;
        MintLoaderLowMapPD[i].Writable = 1;
        MintLoaderLowMapPD[i].Address = ((UINT64)&(MintLoaderLowMapPT[i*512])) >> MM_PAGE_SHIFT;

        for (SIZE_T j = 0; j < 512; j++) {
            MintLoaderLowMapPT[j+(i*512)].Present = 1;
            MintLoaderLowMapPT[j+(i*512)].Writable = 1;
            MintLoaderLowMapPT[j+(i*512)].Address = (((MM_PAGE_SIZE*512)*i) + (MM_PAGE_SIZE * j)) >> MM_PAGE_SHIFT;
        }
    }

    MintLoaderBasePML[0][0].Raw = ((UINT64)&MintLoaderLowMapPDPT) | 0x3;

    /* Using 2MiB pages, identity map */
    for (SIZE_T i = 0; i < MM_PHYSICAL_MAP_SIZE / MM_PAGE_SIZE_LARGE / 512; i++) {
        MintLoaderIdentityMapPDPT[i].Address = ((UINT64)&(MintLoaderIdentityMapPD[i])) >> MM_PAGE_SHIFT;
        MintLoaderIdentityMapPDPT[i].Present = 1;
        MintLoaderIdentityMapPDPT[i].Writable = 1;
        MintLoaderIdentityMapPDPT[i].Usermode = 1;

        for (SIZE_T j = 0; j < 512; j++) {
            MintLoaderIdentityMapPD[i][j].Raw = ((i << 30) + (j << 21)) | 0x80 | 0x03;
        }
    }
    
    /* Set it in the PML */
    MintLoaderBasePML[0][MM_PML4_INDEX(MM_PHYSICAL_MAP_REGION)].Raw = ((UINT64)&MintLoaderIdentityMapPDPT) | 0x7;


    return 0;
}

INT MmArchSetPage(PMINTLDR_PAGE Page, UINT_PTR Address) {
    /* Align the address */
    UINT_PTR AddressAligned = MM_PAGE_ALIGN_DOWN(Address);
    
    /* Get the PML4 entry */
    PMEMORY_PAGE PMLEntry = (PMEMORY_PAGE)&(MintLoaderBasePML[MM_PML4_INDEX(AddressAligned)]);
    if (!PMLEntry->Present) {
        /* The entry is not present, so we need to allocate it */
        UINT_PTR PhysicalBlock = MmAllocatePhysicalPages(1);
        UINT_PTR PhysicalBlockMapped = PhysicalBlock | MM_PHYSICAL_MAP_REGION;
        memset((PVOID)PhysicalBlockMapped, 0, MM_PAGE_SIZE);
        PMLEntry->Address = PhysicalBlock >> MM_PAGE_SHIFT;
        PMLEntry->Present = 1;
        PMLEntry->Writable = 1;
        PMLEntry->Usermode = 1;
    }

    PMEMORY_PAGE PDPT = (PMEMORY_PAGE)((PMLEntry->Address << MM_PAGE_SHIFT) | MM_PHYSICAL_MAP_REGION);

    /* Get the PDPT entry */
    PMEMORY_PAGE PDPTEntry = &(PDPT[MM_PDPT_INDEX(AddressAligned)]);

    if (!PDPTEntry->Present) {
        /* The entry is not present, so we need to allocate it */
        UINT_PTR PhysicalBlock = MmAllocatePhysicalPages(1);
        UINT_PTR PhysicalBlockMapped = PhysicalBlock | MM_PHYSICAL_MAP_REGION;
        memset((PVOID)PhysicalBlockMapped, 0, MM_PAGE_SIZE);
        PDPTEntry->Address = PhysicalBlock >> MM_PAGE_SHIFT;
        PDPTEntry->Present = 1;
        PDPTEntry->Writable = 1;
        PDPTEntry->Usermode = 1;
    }

    PMEMORY_PAGE PD = (PMEMORY_PAGE)((PDPTEntry->Address << MM_PAGE_SHIFT) | MM_PHYSICAL_MAP_REGION);

    /* Get the PD entry */
    PMEMORY_PAGE PDEntry = &(PD[MM_PD_INDEX(AddressAligned)]);

    if (!PDEntry->Present) {
        /* The entry is not present, so we need to allocate it */
        UINT_PTR PhysicalBlock = MmAllocatePhysicalPages(1);
        UINT_PTR PhysicalBlockMapped = PhysicalBlock | MM_PHYSICAL_MAP_REGION;
        memset((PVOID)PhysicalBlockMapped, 0, MM_PAGE_SIZE);
        PDEntry->Address = PhysicalBlock >> MM_PAGE_SHIFT;
        PDEntry->Present = 1;
        PDEntry->Writable = 1;
        PDEntry->Usermode = 1;
    }

    PMEMORY_PAGE PT = (PMEMORY_PAGE)((PDEntry->Address << MM_PAGE_SHIFT) | MM_PHYSICAL_MAP_REGION);
    PMEMORY_PAGE PTEntry = &(PT[MM_PT_INDEX(AddressAligned)]);
    PTEntry->Present = (Page->Flags & MINTLDR_PAGE_PRESENT) ? 1 : 0;
    PTEntry->Writable = (Page->Flags & MINTLDR_PAGE_WRITABLE) ? 1 : 0;
    PTEntry->Usermode = (Page->Flags & MINTLDR_PAGE_USERMODE) ? 1 : 0;
    PTEntry->Address = (Page->Address >> MM_PAGE_SHIFT);
    
    return 0;
}