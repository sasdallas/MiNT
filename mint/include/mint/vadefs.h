/**
 * @file mint/include/mint/vadefs.h
 * @brief va definitions
 * 
 * 
 * @copyright
 * This file is released into the public domain.
 */

#ifndef VALIST_H
#define VALIST_H

/* va_list conversion */
#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
typedef __builtin_va_list va_list;
typedef va_list VA_LIST;
#endif

/* va functions */
#if defined(__GNUC__)
#define va_start(v, l) __builtin_va_start(v, l)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_copy(d, s) __builtin_va_copy(d, s)
#endif

#endif