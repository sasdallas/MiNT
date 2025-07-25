/**
 * @file boot/include/minilibc/string.h
 * @brief libc string.h
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_MINILIBC_STRING_H__
#define __MINT_MINILIBC_STRING_H__ 1

__MINT_BEGIN_DECLS

#include <basetsd.h>

char *itoa(int value, char *str, int base);
void *memset(void *destination, char ch, SIZE_T length);
void *memcpy(void *destination, const void *source, SIZE_T n);
SIZE_T strlen(const char *s);
char *strcpy(char *dest, char *src);
int strcmp(const char* str1, const char* str2);

__MINT_END_DECLS

#endif