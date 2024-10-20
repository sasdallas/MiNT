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


#ifndef DBG_H
#define DBG_H

/* Use a template to prevent errors */
EXTERN
INT
MachPrintSerial(
    PCSTR Format,
    ...
);

/* INFO macro - Used for lightweight information messages */
#define INFO(message)   MachPrintSerial("[INFO] (%s:%i) %s: ", __FILE__, __LINE__, __FUNCTION__); \
                        MachPrintSerial message; \
                        MachPrintSerial("\n");

/* WARN macro - Used for warnings */
#define WARN(message)   MachPrintSerial("[WARN] (%s:%i) %s: ", __FILE__, __LINE__, __FUNCTION__); \
                        MachPrintSerial message; \
                        MachPrintSerial("\n");

/* ERROR macro - Used for errors. If critical is one a bugcheck is done (rhyming) */
#define ERROR(critical, message)    \
        MachPrintSerial("[%s] (%s:%i) %s: ", (critical ? "CRIT" : "ERR ") __FILE__, __LINE__, __FUNCTION__); \
        MachPrintSerial message; \
        MachPrintSerial("\n"); \
        if (critical) MintBugCheckMsg(NULL, __FILE__, __LINE__, message);

/* DEBUG macro - Used for debugging messages */
#ifdef DEBUG
#define DEBUG(message)
#else
#define DEBUG(message)
#endif 

#endif