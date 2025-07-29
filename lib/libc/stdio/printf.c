/**
 * @file lib/libc/stdio/printf.c
 * @brief printf definitions for libc
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 * 
 * @note (Samuel) Some of this file comes from the Ethereal Operating System (which I made :D)
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

/* Output macro */
#define OUT(c) callback(data, (c))


int __xvasprintf_print_integer(unsigned long long value, unsigned int width, int (*callback)(void*,char), void * data, int fill_zero, int align_right, int precision) {
    OUT('%');
    OUT('i');

    return 2; /* TODO */
}

int __xvasprintf_print_hex(unsigned long long value, unsigned int width, int (*callback)(void*,char), void* data, int pad_zero, int print_ptr, int caps, int align) {
    /* Print the hex! */
    int written = 0;
    unsigned long i = width;

    char characters[] = "0123456789abcdef";

    /* Calculate the width of the integer */
    unsigned long long value_rounded = value & ~0xF;
    unsigned long long tmp = 0xF;
    size_t int_width = 1;
    while (value > tmp && tmp < UINT64_MAX) {
        int_width++;
        tmp *= 0x10;
        tmp += 0xF;
    }

    /* Ok, do we need to pad spaces? */
    if (!pad_zero && align == 1) {
        while (i > width + (print_ptr ? 2 : 0)) {
            OUT(' ');
            written++;
            i--;
        }
    }

    /* Should we print out the pointer header? */
    if (print_ptr) { OUT('0'); OUT(caps ? 'X' : 'x'); written += 2; }

    /* Pad zeroes? */
    if (pad_zero && align == 1) {
        while (i > int_width + (print_ptr ? 2 : 0)) {
            OUT('0');
            written++;
            i--;
        }
    }

    /* Print each character */
    i = int_width;
    while (i--) {
        char ch = characters[(value >> (i * 4)) & 0xF];
        OUT(caps ? ((ch >= 'a' && ch <= 'z' ? ch - 0x20 : ch)) : ch);
        written++;
    }

    /* Right-pad */
    if (align == 0) {
        i = width;
        while (i > int_width + (print_ptr ? 2 : 0)) {
            OUT(' ');
            written++;
            i--;
        }
    }

    return written;
}

int __xvasprintf(int (*callback)(void*, char), void *data, const char *fmt, va_list args) {
    int written = 0;
    char *f = fmt;
    while (*f) {
        if (*f != '%') {
            OUT(*f);
            f++;
            written++;
            continue;
        }

        /* We found a percentage sign, skip past it */
        f++;

        if (*f == '%') {
            OUT(*f);
            f++;
            written++;
            continue;
        }

		#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

		// Start parsing flags
		int justification = 0;		// 0 = right, 1 = left
		int sign_type = 0;			// 0 = sign only on negative, 1 = sign always, 2 = sign never
		char padding = ' ';			// Default padding is with spaces
		int add_special_chars = 0;	// Depends on specifier
		while (*f) {
			int finished_reading = 0;

			switch (*f) {
				case '-':
					// Left-justify width. This means that we will pad after the value, instead of before. 
					justification = 1;
					break;

				case '+':
					// Always sign
					sign_type = 1;
					break;

				case ' ':
					// Never sign
					sign_type = 2;
					break;

				case '0':
					// Left-pad with zeroes instead of spaces when padding is specified in width.
					padding = '0';
					break;

				case '#':
					// This is a special case. If used with %o or %x it adds a 0/0x - if used with %a/%e/%f/%g it forces decimal points
					add_special_chars = 1;
					break;

				default:
					// Do nothing
					finished_reading = 1;
					break;
			}

			if (finished_reading) break;
			f++;
		}

		// Width should be read indefinitely as long as the characters are available
		// TODO:  ... better way?
		int width = 0;

		// Special case: Width is provided in va_args
		if (*f == '*') {
			width = va_arg(args, int);
			f++;
		} else  {
			while ((*f >= '0' && *f <= '9')) {
				// Add this to the width
				width *= 10;
				width += (int)((*f) - '0');
				f++;
 			}
		}

		// Do we need to process precision?
		int precision = -1; // Precision will be auto-updated when we handle
		if (*f == '.') {
			// Yes we do!
			precision = 0;
			f++;

			// For integer specifiers (d, i, o, u, x) precision specifies the minimum number of digits to be written.
			// If it shorter than this, it is padded with leading zeroes.
			// For %a/%e/%f this is the number of digits to printed AFTER the decimal point (default 6)
			// For %g this is the maximum number of significant digits
			// For %s this is the maximum number of characters to print

			// If it is '*' grab it from VA arguments
			if (*f == '*') {
				precision = va_arg(args, int);
				f++;
			} else {
				// Keep reading the precision
				while ((*f >= '0' && *f <= '9')) {
					// Add this to the width
					precision *= 10;
					precision += (int)((*f) - '0');
					f++;
				}
			}
		}

		// Length modifies the length of the data type being interpreted by VA arguments.
		// There's a nice table available at https://cplusplus.com/reference/cstdio/printf/.
		// Each specifier interprets length differently, so I'll use a value to show that
		
		/* Starting from 0, length represents: (none), h, hh, l, ll, j, z, t, L */
		int length = 0;		// By default, 0 is none
		switch (*f) {
			case 'h':
				length = 1;
				f++;
				if (*f == 'h') {
					length = 2;
					f++;
				}
				break;
			case 'l':
				length = 3;
				f++;
				if (*f == 'l') {
					length = 4;
					f++;
				}
				break;
			case 'j':
				length = 5;
				f++;
				break;
			case 'z':
				length = 6;
				f++;
				break;
			case 't':
				length = 7;
				f++;
				break;
			case 'L':
				length = 8;
				f++;
				break;
			default:
				break;
		}

        
		// Now we're on to the format specifier.
		// TODO: Support for %g, %f
		switch (*f) {
			case 'd':
			case 'i': ;
				// %d / %i: Signed decimal integer
				long long dec;

				// Get value
				if (length == 6) {
					dec = (size_t)(va_arg(args, size_t));
				} else if (length == 5) {
					dec = (intmax_t)(va_arg(args, intmax_t));
				} else if (length == 4) {
					dec = (long long int)(va_arg(args, long long int));
				} else if (length == 3) {
					dec = (long int)(va_arg(args, long int));
				} else {
					// The remaining specifiers are all promoted to integers regardless
					dec = (int)(va_arg(args, int));
				}

				if (dec < 0) {
					// Handle negative sign
					if (sign_type != 2) {
						OUT('-');
					} 

					// Invert
					dec = -dec;
				} else if (sign_type == 1) {
					// Always sign
					OUT('+');
				}

				written += __xvasprintf_print_integer(dec, width, callback, data, (padding == '0'), !justification, precision);
				break;
			
			case 'p': ;
				// %p: Pointer 
				unsigned long long ptr;
				if (sizeof(void*) == sizeof(long long)) {
					ptr = (unsigned long long)(va_arg(args, unsigned long long));
				} else {
					ptr = (unsigned int)(va_arg(args, unsigned int));
				}
				written += __xvasprintf_print_hex(ptr, width, callback, data, (padding == '0'), 1, (*f >= 'A' && *f <= 'Z'), !justification);
				break;
			
			case 'x':
			case 'X': ;
				// %x: Hexadecimal format
				unsigned long long hex;
				if (length == 6) {
					hex = (size_t)(va_arg(args, size_t));
				} else if (length == 5) {
					hex = (uintmax_t)(va_arg(args, uintmax_t));
				} else if (length == 4) {
					hex = (unsigned long long int)(va_arg(args, unsigned long long int));
				} else if (length == 3) {
					hex = (unsigned long int)(va_arg(args, unsigned long int));
				} else {
					// The remaining specifiers are all promoted to integers regardless
					hex = (unsigned int)(va_arg(args, unsigned int));
				}

				written += __xvasprintf_print_hex(hex, width, callback, data, (padding == '0'), add_special_chars, (*f >= 'A' && *f <= 'Z'), !justification);
				break;

			case 'u': ;
				// %u: Unsigned
				unsigned long long uns;
				if (length == 6) {
					uns = (size_t)(va_arg(args, size_t));
				} else if (length == 5) {
					uns = (uintmax_t)(va_arg(args, uintmax_t));
				} else if (length == 4) {
					uns = (unsigned long long int)(va_arg(args, unsigned long long int));
				} else if (length == 3) {
					uns = (unsigned long int)(va_arg(args, unsigned long int));
				} else {
					// The remaining specifiers are all promoted to integers regardless
					uns = (unsigned int)(va_arg(args, unsigned int));
				}

				written += __xvasprintf_print_integer(uns, width, callback, data, (padding == '0'), !justification, precision);
				break;

			case 's': ;
				// %s: String
				char *str = (char*)(va_arg(args, char*));
				if (str == NULL) {
					// Nice try
					str = "(NULL)";
				}

				// Padding applies
				int chars_printed = 0;

				// Does it have precision?
				if (precision >= 0) {
					for (int i = 0; i < precision && *str; i++) {
						OUT(*str);
						str++;
						chars_printed++;
					}
				} else {
					while (*str) {
						OUT(*str);
						str++;
						chars_printed++;
					}
				}

				// Does it have width?
				if (chars_printed < width) {
					for (int i = chars_printed; i < width; i++) {
						// Add some padding
						OUT(padding);
					}
				}
				break;
			
			case 'c': ;
				// %c: Singled character
				OUT((char)(va_arg(args, int)));
				break;

			default: ;
				OUT(*f);
				break;
        }

        f++;
    }
}


struct __string_data {
    char *str;          // String to use
    long size;          // Size of string
    long idx;           // Index in string we are in
};

static int __xvasprintf_sprintf_callback(void *user, char ch) {
    struct __string_data *str = (struct __string_data*)user;

    if (str->size > str->idx || str->size == -1) {
        str->str[str->idx] = ch;
        str->idx++;
        
        /* Enough space for a null termination? */
        if (str->size > str->idx || str->size == -1) {
            str->str[str->idx] = 0;
        }
    }

    return 0;
}

int vsnprintf (char * s, size_t n, const char * format, va_list arg ) {
    struct __string_data data = {
        .str = s,
        .size = n,
        .idx = 0
    };

    return __xvasprintf(__xvasprintf_sprintf_callback, &data, format, arg);
}

int vsprintf(char *s, const char *format, va_list arg) {
    struct __string_data data = {
        .str = s,
        .size = -1,
        .idx = 0
    };

    return __xvasprintf(__xvasprintf_sprintf_callback, &data, format, arg);
}

