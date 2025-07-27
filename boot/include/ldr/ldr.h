/**
 * @file boot/include/ldr/ldr.h
 * @brief PE loader
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_LDR_LDR_H__
#define __MINTLDR_LDR_LDR_H__ 1

__MINT_BEGIN_DECLS

#include <winnt.h>
#include <mm/region.h>

typedef struct MINTLDR_IMAGE_EXPORT {
    PCHAR Name;                                 // Name of the export
    UINT_PTR Address;                           // Address of the export
    struct MINTLDR_IMAGE_EXPORT *Next;          // Next export
} MINTLDR_IMAGE_EXPORT, *PMINTLDR_IMAGE_EXPORT;

typedef struct MINTLDR_LOADED_IMAGE {
    PCHAR Name;                                 // Name of the loaded image
    PVOID LoadBase;                             // Base of the loaded image
    PVOID Entrypoint;                           // Rebased entrypoint
    PMINTLDR_IMAGE_EXPORT ImageExportList;      // Image export list
    struct MINTLDR_LOADED_IMAGE  *NextImage;    // Next image in list
    struct MINTLDR_LOADED_IMAGE  *PrevImage;    // Previous image in list
} MINTLDR_LOADED_IMAGE, *PMINTLDR_LOADED_IMAGE;

BOOL LdrFindDllByName(PCHAR DllName, PMINTLDR_LOADED_IMAGE *LoadedImage);
BOOL LdrCheckIfDllLoaded(PCHAR DllName);
INT LdrImageLoadEx(PCHAR ImageName, UINT_PTR Base, MINTLDR_MEMORY_TYPE MemoryType, PVOID *ImageBase, PMINTLDR_LOADED_IMAGE *LoadedImageOut);
INT LdrImageLoad(PCHAR ImageName, UINT_PTR Base, MINTLDR_MEMORY_TYPE MemoryType, PVOID *ImageBase);
UINT_PTR LdrImageGetEntrypoint(UINT_PTR ImageBase, PVOID LoadBase);

/* Architecture must implement these */
UINT_PTR LdrArchImageLookup(PCHAR ImageName);

__MINT_END_DECLS

#endif 