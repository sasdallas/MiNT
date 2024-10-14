/**
 * @file mintldr/minilib/string.c
 * @brief Mini C library string functions
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <minilib/string.h>
#include <mintldr.h>

/* Memory set */
PVOID
memset(
    PVOID Destination,
    CHAR Character,
    SIZE_T Length
)
{
    return NULL;
}

/* String length */
INT
strlen(
    PCSTR String
)
{
    INT Length = 0;
    while (String[Length])
    {
        Length += 1;
    }
    return Length;
}


/* String copy */
/* TODO: Constantize source (but GCC no likey) */
char*
strcpy(
    _Out_ char* Destination,
    _In_ CONST char* Source
)
{
    char* DestinationPtr = Destination;
    while (*Source) {
        *Destination = *Source;

        Destination++;
        Source++;
    }

    return DestinationPtr;
}