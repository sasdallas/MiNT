/**
 * @file mintldr/ui/ui_print.c
 * @brief UI print driver
 * 
 * 
 * @copyright
 * This file is part of MiNT, which is created by Samuel.
 * It is released under the terms of the BSD 3-clause license.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2024 Samuel S.
 */

#include <mintldr.h>
#include <ui/ui.h>

#include <vadefs.h>

/* This file has all the 'printf' style functions */


static
BOOL
UiPrintData(
    PCSTR Data,
    SIZE_T Length
)
{
    for (SIZE_T i = 0; i < Length; i++) 
    {
        UiPutCharacter(((PUCHAR)Data)[i]);
    }

    return TRUE;
}

INT
UiPrint(
    PCSTR Format, ...
) 
{
    va_list ap;
    va_start(ap, Format);

    /* The amount of characters written */
    SIZE_T CharactersWritten;

    /* Argument width, or padding */
    INT ArgumentWidth = 0;

    while (*Format) {
        /* Reset argument width */
        ArgumentWidth = 0;

        if (Format[0] != '%' || Format[1] == '%')
        {
            if (Format[0] == '%')
            {
                Format++;
            }

            /* Calculate characters until next % */
            SIZE_T CharactersUntilFmt = 0;
            while (Format[CharactersUntilFmt] && Format[CharactersUntilFmt] != '%') 
            {
                CharactersUntilFmt++;
            }

            /* Print out */
            if (UiPrintData(Format, CharactersUntilFmt) != TRUE) 
            {
                return -1;
            }

            Format += CharactersUntilFmt;
            CharactersWritten += CharactersUntilFmt;
            continue;
        }        

        /* Now we should check the % */
        const PCSTR FormatStart = Format++;

        while (*Format >= '0' && *Format <= '9')
        {
            ArgumentWidth *= 10;
            ArgumentWidth += *Format - '0';
            *Format++;
        }

        switch (*Format) {
            
            /* %c - Character */
            case 'C':
            case 'c':
                Format++;
                CHAR c = (CHAR)va_arg(ap, INT);

                if (UiPrintData(&c, sizeof(c)) != TRUE) 
                {
                    return -1;
                }

                CharactersWritten++;

                break;
            
            /* %s - String */
            case 'S':
            case 's':
                Format++;
                const char* Str = va_arg(ap, const char*);
                SIZE_T StrLength = strlen(Str);

                if (UiPrintData(Str, StrLength) != TRUE) 
                {
                    return -1;
                }

                CharactersWritten += StrLength;

                break;

            /* %i - Integer */
            case 'I':
            case 'i': {
                Format++;
                INT Integer = va_arg(ap, INT);
                CHAR StringBuffer[32] = {0};

                /* Convert integer to string, assuming base-10 */
                itoa(Integer, StringBuffer, 10);

                /* Calculate string length */
                SIZE_T BufferLength = strlen(StringBuffer);

                /* If an argument length was specified, deal with that */
                if (ArgumentWidth && ArgumentWidth > BufferLength)
                {
                    CHAR PaddingBuffer[32] = {0};
                    for (INT i = 0; i < ArgumentWidth - BufferLength; i++)
                    {
                        PaddingBuffer[i] = '0';
                    }

                    if (UiPrintData(&StringBuffer, BufferLength) != TRUE)
                    {
                        return -1;
                    }
                }

                if (UiPrintData(&StringBuffer, BufferLength) != TRUE)
                {
                    return -1;
                }

                CharactersWritten += BufferLength;

                break;
            }

            /* %u - Unsigned */
            /* %x - Hexadecimal */
            case 'U':
            case 'u':
            case 'X':
            case 'x': {
                Format++;
                unsigned int Value = va_arg(ap, unsigned int);
                CHAR StringBuffer[32] = {0};

                /* Convert hex number to string, assuming base-16 */
                itoa(Value, StringBuffer, 16);

                /* Calculate string length */
                SIZE_T BufferLength = strlen(StringBuffer);

                /* If an argument length was specified, deal with that */
                if (ArgumentWidth && ArgumentWidth > BufferLength)
                {
                    CHAR PaddingBuffer[32] = {0};
                    for (INT i = 0; i < ArgumentWidth - BufferLength; i++)
                    {
                        PaddingBuffer[i] = '0';
                    }

                    if (UiPrintData(&PaddingBuffer, ArgumentWidth - BufferLength) != TRUE)
                    {
                        return -1;
                    }
                }

                if (UiPrintData(&StringBuffer, BufferLength) != TRUE)
                {
                    return -1;
                }

                CharactersWritten += BufferLength;

                break;
            }
            
            /* Anything else */
            default:
                Format = FormatStart;
                SIZE_T WriteLength = strlen(Format);
                
                if (UiPrintData(Format, WriteLength) != TRUE)
                {
                    return -1;
                }

                Format += WriteLength;
                CharactersWritten += WriteLength;
                break;
        }
    }

    va_end(ap);
    return CharactersWritten;
}
