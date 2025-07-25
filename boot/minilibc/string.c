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

void *memcpy(void *destination, const void *source, SIZE_T n) {
    char *dst = (char *)destination;
    const char *src = (const char *)source;

    for (SIZE_T i = 0; i < n; i++) {
        dst[i] = src[i];
    }

    return destination;
}

SIZE_T strlen(const char *s) {
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

int strcmp(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }

    return *(unsigned char*)str1 - *(unsigned char*)str2;
}