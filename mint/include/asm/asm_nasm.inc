; =======================================================================================================
; FILE:         asm_nasm.inc
; DESCRIPTION:  Main assembly macro file
; AUTHOR:       Samuel S.
; CREATED:      October 9th, 2024
; LICENSE:      Under the license of MiNT (BSD 3-clause)
; =======================================================================================================

; This file is pretty barren right now. Will add more to it later.

; ==== CONSTANTS
%define quot "
%define syntax_highlighting_bad " ; This is because VS code really does not like syntax highlighting with an unterminated quote

; ==== HELPER MACROS

; STR is designed to be used in accordance with %strcat
%define STR(x) quot %+ x %+ quot


