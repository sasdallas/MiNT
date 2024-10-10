// =======================================================================================================
// FILE:         asm.inc
// DESCRIPTION:  Main assembly macro file
// AUTHOR:       Samuel S.
// CREATED:      October 9th, 2024
// LICENSE:      Some components are present from other OSes/projects.
//               Because of this, normal BSD-clause licensing for this file DOES NOT APPLY.
//               In accordance with GPL, this file is open-source and free to use.
//               ReactOS                  See EXTERNAL/ReactOS.LICENSE (GPL)
// =======================================================================================================


#ifndef     __ASM_INC__
#define     __ASM_INC__

/* Conversion */
#define BIN(x) 0b##x
#define OCT(x) 0##x
#define DEC(x) x
#define HEX(x) 0x##x

/* Macro value */
#define VAL(x) \x 


/* String variables */
#define CR 13
#define LF 10
#define NUL 0


#endif 