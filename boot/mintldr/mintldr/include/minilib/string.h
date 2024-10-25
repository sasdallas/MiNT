/**
 * @file mintldr/include/minilib/string.h
 * @brief Minilib string functions header file
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#ifndef MINILIB_STRING_H
#define MINILIB_STRING_H

#include <WinDef.h>

/* String length */
INT
strlen(
    _In_ PCSTR String
);

/* Copy string */
char*
strcpy(
    _Out_ char* Destination,
    _In_ CONST char* Source
);

/* Memory set */
PVOID
memset(
    _In_ PVOID Destination,
    _In_ CHAR Character,
    _In_ SIZE_T Length
);

/* Integer to string */
VOID
itoa(
    _In_ PVOID Input, 
    _In_ PCHAR Buffer,
    _In_ INT IntBase
);



#endif