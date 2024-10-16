/**
 * @file mintldr/include/dbg.h
 * @brief This file provides debugging macros such as WARN, DEBUG, etc.
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

/* This is currently just stubs because mach doesn't have serial output */

#ifndef DBG_H
#define DBG_H

/* INFO macro - Used for lightweight information messages */
/* It's best to use the mach log function instead of this */
#define INFO(message)

/* WARN macro - Used for warnings */
#define WARN(message)

/* ERROR macro - Used for errors. If critical is one a bugcheck is done (rhyming) */
#define ERROR(critical, message)

/* DEBUG macro - Used for debugging messages */
#ifdef DEBUG
#define DEBUG(message)
#else
#define DEBUG(message)
#endif 

#endif