/**
 * @file boot/include/dbg.h
 * @brief Boot debug header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINTLDR_DBG_H__
#define __MINTLDR_DBG_H__ 1

__MINT_BEGIN_DECLS

#include <arch/serial.h>

#define LOG(module, ...)    SerialPrint("[%s] (%s:%i) %s: ", module, __FILE__, __LINE__, __FUNCTION__); \
                            SerialPrint(__VA_ARGS__)

#define INFO(...) LOG("INFO ", __VA_ARGS__)
#define WARN(...) LOG("WARN ", __VA_ARGS__)
#define ERROR(...) LOG("ERROR", __VA_ARGS__)
#define DEBUG(...) LOG("DEBUG", __VA_ARGS__)

__MINT_END_DECLS

#endif