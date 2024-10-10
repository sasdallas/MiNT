# =========================================================================
# FILE:         fat1216.S
# DESCRIPTION:  FAT 12/16 boot sector
# AUTHOR:       Samuel S.
# CREATED:      October 9th, 2024
# LICENSE:      Under the license of MiNT (BSD 3-clause)
# =========================================================================

#include <asm.inc>

// Code starts at 7C00h

.code16

// EB 3C 90 signature (if you get lucky)
start: 
    jmp main
    nop


// BPB (BIOS Parameter Block)
OEMName:
    .ascii "MiNT_LDR"
BytesPerSector:
    .word 512
SectorsPerCluster:
    .byte 1
ReservedSectors:
    .byte 1
NumberOfFats:
    .byte 2
MaxRootEntries:
    .word 224
TotalSectors:
    .word 2880
MediaDescriptor:
    .byte HEX(0f0)
SectorsPerFat:
    .word 9
SectorsPerTrack:
    .word 18
NumberOfHeads:
    .word 2
HiddenSectors:
    .long 0
TotalSectorsBIg:
    .long 0
BootDrive:
    .byte HEX(0ff)
Reserved:
    .byte 0
ExtendSig:
    .byte HEX(29)
SerialNumber:
    .long 00000000
VolumeLabel:
    .ascii "MiNT       "
FileSystem:
    .ascii "FAT12   "


// Print string function
PrintString:
    lodsb
    orb %al, %al
    jz PrintDone
    movb HEX(0E), %ah
    movw $7, %bx
    int $0x10
    jmp PrintString 

PrintDone:
    ret

main:

    cli
    hlt



// String variables
msg_hello:
    .ascii "MINTLDR"
    .byte CR
    .byte LF
    .byte NUL

// Padding to 509 bytes
.org 509

BootPartition: 
    .byte 0

BootSignature:
    .word HEX(0aa55) // Bootsector signature for BIOS
