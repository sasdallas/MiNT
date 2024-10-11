; =========================================================================
; FILE:         fat.asm
; DESCRIPTION:  FAT 12/16 boot sector (NASM syntax)
; AUTHOR:       Samuel S.
; CREATED:      October 9th, 2024
; LICENSE:      Under the license of MiNT (BSD 3-clause)
; =========================================================================

%include "asm_nasm.inc"

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

; main - Entrypoint for MINTLDR
; Parameters:
;       None
; Returns:
;       Nothing

main:
    ; Setup a stack first
    xor ax, ax              ; Zero out data registers (real mode)
    mov ds, ax
    mov es, ax

    mov bp, 0x7C00          ; Using rebased BPB is best practice
    mov sp, 0x7C00-16       ; Starting stack at 0x7C00-16

    ; Save DL
    mov dl, byte [BootDrive]


    
    mov si, loading_string
    call PrintString

    cli
    hlt


; PrintString - Print a string out
; Parameters:
;       SI - String
; Returns:
;       Nothing

PrintString:
    lodsb                   ; Load character from SI
    or al, al               ; Is it zero?
    jz .PrintString_done    ; Yes, we're done
    mov ah, 0Eh             ; Specify teletype printing
    int 0x10                ; Call BIOS
    jmp PrintString         ; Loop again
.PrintString_done:
    ret

; ReadSectors - Read sectors from the disk
; Parameters:
;       ES:BX - Buffer to read to
;       CX - Number of sectors
;       AX - Starting sector
; Returns:
;       Nothing
; TODO:
;       This function should be upgraded to support LBA addressing as well as CHS.
;       This function is mostly "borrowed", no likey.

ReadSectors: 
.read_sector_start:
    mov di, 5                           ; Allow 5 retries for error
.sector_loop: 
    push ax                             ; Save copies of parameters
    push bx
    push cx
    call LBA2CHS                        ; Convert AX (LBA address) to CHS address

    ; Setup parameters & call BIOS
    mov ah, 0x02                        ; 0x02 - BIOS read sector
    mov al, 0x01                        ; Read one sector (we'll loop this)
    mov ch, byte [AbsoluteTrack]        ; Track
    mov cl, byte [AbsoluteSector]       ; Sector
    mov dh, byte [AbsoluteHead]         ; Head
    mov dl, byte [BootDrive]            ; Drive number
    int 0x13                            ; Call BIOS
    
    ; Check for success
    jnc .success                        ; Successfully read a sector

    ; No success. Reset disk, decrement error counter and try again
    xor ax, ax                          ; 0x00 - BIOS reset disk
    int 0x13                            ; Call BIOS

    dec di                              ; Decrement error counter
    pop cx                              ; Pop variables from stack
    pop bx
    pop ax
    jnz .sector_loop                    ; Is DI = 0? If not, try again.

    ; DI = 0. Use INT 0x18 to signify there is no bootable disk
    int 0x18
.success:
    pop cx
    pop bx
    pop ax
    add bx, word [bpb_BytesPerSector]   ; Add bytes to BX (ES:BX)
    inc ax
    loop .read_sector_start

    


; LBA2CHS - Convert an LBA address to a CHS address
; Parameters: 
;       AX - LBA sector address
; Returns:
;       Absolute sector
;       Absolute head
;       Absolute track

LBA2CHS:
    ; Calculate sector
    xor dx, dx                      ; Clear out DX, we'll use it
    div word [bpb_SectorsPerTrack]  ; Divide by the sectors per track
    inc dl                          ; Zero indexed
    mov byte [AbsoluteSector], dl   ; Store in AbsoluteSector
    
    ; Calculate the absolute head (AX contains the result from sector calculation)
    xor dx, dx
    div word [bpb_NumberOfHeads]
    mov byte [AbsoluteHead], dl

    ; AL contains the result for our absolute track
    mov byte [AbsoluteTrack], al
    ret


    


; **************** DATA START ***************** 

%ifdef VERSION_STR
%strcat loadStr "MINTLDR v", STR(VERSION_STR)
loading_string:
    db loadStr, 13, 10, 0
%else
loading_string:
    db "MINTLDR", 13, 10, 0
%endif


; Boot parameter variables
BootDrive           db 0

; CHS variables
AbsoluteSector      db 0     
AbsoluteHead        db 0
AbsoluteTrack       db 0

; Drive variables
DataSector          dw 0x0000
Cluster             dw 0x0000


; Final boot signature
times 510-($-$$) db 0
dw 0xAA55