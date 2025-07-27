/**
 * @file include/mint/hal.h
 * @brief HAL definitions (NTHALAPI + friends) 
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_HAL_H__
#define __MINT_HAL_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>

#if defined(_MINTHAL_) || defined(_NTHAL_)
#define NTHALAPI 
#else
#define NTHALAPI __declspec(dllimport)
#endif

#include <ddk/ioaccess.h>

__MINT_END_DECLS

#endif