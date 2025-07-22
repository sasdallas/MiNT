/**
 * @file boot/minilibc/string.c
 * @brief Mini libc string functions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <minilibc/string.h>

void *memset(void *destination, char ch, SIZE_T n) {
    SIZE_T i = 0;
    for (; i < n; i++) {
        ((char*)destination)[i] = ch;
    }
    return destination;
}

int strlen(char *s) {
    int len = 0;
    while (*s++) len++; 
    return len;
}

char *strcpy(char *dst, char *src) {
    char *dst_p = dst;

    while (*src) {
        *dst++ = *src++;
    }

    return dst;
}