; =========================================================================
; FILE:         fat.asm
; DESCRIPTION:  FAT 12/16 boot sector (NASM syntax)
; AUTHOR:       Samuel S.
; CREATED:      October 9th, 2024
; LICENSE:      Under the license of MiNT (BSD 3-clause)
; =========================================================================


; Code starts at 7C00h
BITS 16
org 0x7C00

; EB 3C 90 signature if you get lucky
start:
    jmp main
    nop

; ***************** BPB BLOCK *******************

bpb_OemName:            db "MINTLDR "
bpb_BytesPerSector:     dw 512
bpb_SectorsPerCluster:  db 1
bpb_ReservedSectors:    dw 1
bpb_FATs:               db 2
bpb_RootEntries:        dw 224
bpb_TotalSectors:       dw 2880
bpb_MediaDescriptor:    db 0xF0
bpb_SectorsPerFAT:      dw 9
bpb_SectorsPerTrack:    dw 18
bpb_NumberOfHeads:      dw 2
bpb_HiddenSectors:      dd 0
bpb_TotalSectorsBig:    dd 0

; **************** EBPB BLOCK ******************
; This conforms to DOS 4.0 specifications
ebpb_DriveNumber:       db 0
ebpb_Unused:            db 0
ebpb_ExtBootSignature:  db 0x29
ebpb_SerialNumber:      dd 0x00000000
ebpb_VolumeName:        db "NO NAME    "
ebpb_FileSystem:        db "FAT12   "


; ***************** CODE START ***************** 


PrintString:
    lodsb                   ; Load character from SI
    or al, al               ; Is it zero?
    jz .PrintString_done    ; Yes, we're done
    mov ah, 0Eh             ; Specify teletype printing
    int 0x10                ; Call BIOS
    jmp PrintString         ; Loop again
.PrintString_done:
    ret


main:
    ; Setup a stack first
    xor ax, ax              ; Zero out data registers (real mode)
    mov ds, ax
    mov es, ax

    mov sp, 0x7C00          ; Starting stack at 0x7C00
    
    mov si, loading_string
    call PrintString

    cli
    hlt



; **************** DATA START ***************** 

loading_string:
    db "MINTLDR", 13, 10, 0


; Final boot signature

times 510-($-$$) db 0
dw 0xAA55