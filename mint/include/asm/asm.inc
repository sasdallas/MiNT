// =======================================================================================================
// FILE:         asm.inc
// DESCRIPTION:  Main assembly macro file
// AUTHOR:       Samuel S.
// CREATED:      October 9th, 2024
// LICENSE:      Under the license of MiNT (BSD 3-clause)
// =======================================================================================================


#ifndef     __ASM_INC__
#define     __ASM_INC__

/* Specify to use Intel syntax (AT&T sucks, deal with it) */
.intel_syntax noprefix

/* Conversion */
#define BIN(x) 0b##x
#define OCT(x) 0##x
#define DEC(x) x
#define HEX(x) 0x##x

/* Macro value */
#define VAL(x) \x 


/* String variables */
#define CR  "\r"
#define LF  "\n"
#define NUL "\0"


#endif 