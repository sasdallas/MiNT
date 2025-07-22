/**
 * @file boot/minilibc/stdlib.c
 * @brief Standard library for mini libc
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <minilibc/stdlib.h>

/* The source code here was sourced from the Ethereal Operating System. I own Ethereal. */

char *itoa(int value, char *str, int base) {
    char *str_p = str;
    static char substitutes[] = {"FEDCBA9876543210123456789ABCDEF"};
    char temporary[32];
    int pos = 0;
    int opos = 0;
    int top = 0;

    /* Typecast to unsigned integer to handle large hex numbers */
    unsigned int number = (unsigned int)value;

    /* Prepend output buffer with '-' if negative (only for base 10) */
    if ((int)number < 0 && base == 10) {
        *str++ = '-';
        number = (unsigned int)(-(int)number);
    }

    /* Don't bother if the number is 0 or base > 16 */
    if (!number || base > 16) {
        *str++ = '0';
        *str++ = 0;
        return str;
    }

    /* Convert number to string in temporary buffer */
    while (number) {
        temporary[pos] = substitutes[15 + number % base];
        pos++;
        number /= base;
    }

    /* Adjust top */
    top = pos--;

    /* Copy to main buffer */
    for (; opos < top; pos--,opos++) {
        str[opos] = temporary[pos];
    }

    str[opos] = 0;
    return str_p;
}
