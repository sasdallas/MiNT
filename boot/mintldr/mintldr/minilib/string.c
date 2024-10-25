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
    SIZE_T i = 0;
    for (; i < Length; i++)
    {
        ((PCHAR)Destination)[i] = Character;
    }
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
/* TODO: Make these Windows types (but GCC no likey) */
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

/* Integer to String */
VOID
itoa(
    _In_ PVOID Input, 
    _In_ PCHAR Buffer,
    _In_ INT IntBase
)
{
    static CHAR SubstituteCharacters[] = {"FEDCBA9876543210123456789ABCDEF"};
    CHAR TemporaryBuffer[32];
    INT CurrentPos = 0;
    INT OPos = 0;
    INT Top = 0;

    /* Typecast to unsigned integer to handle large hex numbers */
    UINT Number = (UINT)Input;

    /* Prepend output buffer with '-' if negative (only for base 10) */
    if ((INT)Number < 0 && IntBase == 10)
    {
        *Buffer++ = '-';
        Number = (UINT)(-(INT)Number); /* Ugly typecast */
    }

    /* Don't bother if the number is zero or the base is bigger than 16 (unsupported) */
    if (Number == 0 || IntBase > 16)
    {
        Buffer[0] = '0';
        Buffer[1] = '\0';
        return;
    }

    /* Convert number to string into temporary buffer */
    while (Number != 0) {
        TemporaryBuffer[CurrentPos] = SubstituteCharacters[15 + Number % IntBase];
        CurrentPos++;
        Number /= IntBase;
    }

    /* Adjust top */
    Top = CurrentPos--;

    /* Copy to main buffer */
    for (OPos = 0; OPos < Top; CurrentPos--,OPos++)
    {
        Buffer[OPos] = TemporaryBuffer[CurrentPos];
    }

    /* Null terminate */
    Buffer[OPos] = 0;
}
