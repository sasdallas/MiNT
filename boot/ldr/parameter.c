/**
 * @file boot/ldr/parameter.c
 * @brief Handles placing the loader parameter block
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <ldr/ldr.h>
#include <mm/mm.h>
#include <arc.h>
#include <minilibc/string.h>


INT LdrImageCreateLoaderBlock(PMINTLDR_LOADED_IMAGE LoadedImage, PLOADER_PARAMETER_BLOCK *LoaderBlock) {
    /* Hopefully this actually works. If DLLs are too close to the kernel, this entire thing falls apart */

    UINT_PTR End = MM_PAGE_ALIGN_UP((UINT_PTR)LoadedImage->LoadBase + LoadedImage->Size);

    /* First allocation is for the kernel stack */
    PMINTLDR_MEMORY_REGION MemoryRegion = NULL;
    UINT_PTR KernelStack = MmAllocatePagesAtAddress(LDR_DEFAULT_KERNEL_STACK_SIZE / MM_PAGE_SIZE, End, &MemoryRegion);
    MemoryRegion->MemoryType = RegionKernelStack;

    /* Next allocation is for the parameter block */
    End += MM_PAGE_ALIGN_UP(LDR_DEFAULT_KERNEL_STACK_SIZE);
    PLOADER_PARAMETER_BLOCK LoaderParameterBlock = (PLOADER_PARAMETER_BLOCK)MmAllocatePagesAtAddress(2, End, &MemoryRegion);
    memset(LoaderParameterBlock, 0, sizeof(LOADER_PARAMETER_BLOCK));
    
    /* Set parameters */
    LoaderParameterBlock->KernelStack = KernelStack;
    LoaderParameterBlock->KernelStackSize = LDR_DEFAULT_KERNEL_STACK_SIZE;
    LoaderParameterBlock->ArcBootDeviceName = "multi(0)disk(0)rdisk(0)partition(0)";

    LoadedImage->Size += (2 * MM_PAGE_SIZE) + LDR_DEFAULT_KERNEL_STACK_SIZE;
    if (LoaderBlock) *LoaderBlock = LoaderParameterBlock;
    return 0;
}