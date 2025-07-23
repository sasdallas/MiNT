; MINTLDR stub for booting from Multiboot

; Multiboot header

MULTIBOOT_PAGE_ALIGN        equ 1 << 0
MULTIBOOT_MEMINFO           equ 1 << 1
MULTIBOOT_AOUT_KLUDGE       equ 1 << 16

MULTIBOOT_FLAGS             equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_AOUT_KLUDGE | MULTIBOOT_MEMINFO
MULTIBOOT_MAGIC             equ 0x1BADB002
MULTIBOOT_CHECKSUM          equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

; External symbols
extern __text_start
extern __data_end
extern __bss_end

section .multiboot
align 4

; Multiboot1 header
MintLoaderMultiboot1Header:
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

    ; a.out kludge header for booting a PE file
MintLoaderAoutKludge:
    dd MintLoaderMultiboot1Header
    dd __text_start
    dd __data_end
    dd __bss_end
    dd MintLoaderEntry


; ========== STACK ==========
section .bss
align 16
MintLoaderStackBottom:
resb 16384
MintLoaderStackTop:

; ========== START ==========

[bits 32]
section .text

; ==== GDT ==== 

align 8

; GDT pointer
MintLoaderBaseGdtr:
    dw GdtBaseEnd - GdtBaseStart
    dq GdtBaseStart


; GDT data
GdtBaseStart:
    ; NULL segment
    dd 0
    dd 0

    ; Code segment - 64-bit kernel mode
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0x20
    db 0x00

    ; Data segment - 64-bit kernel mode
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0x00
    db 0x00
GdtBaseEnd:


global MintLoaderEntry
global MintLoaderEntry64
extern MintLoaderBasePML

extern _MintLoaderMain
MintLoaderEntry:
    mov esp, MintLoaderStackTop

    ; 16-byte align the stack
    and esp, 0xFFFFFFF0

    ; Clear EBP
    xor ebp, ebp

    ; Push our multiboot information
    push dword 0
    push esp
    push dword 0
    push eax
    push dword 0
    push ebx

MintLoaderInitializePaging:
    mov edi, MintLoaderBasePML
    
    ; Set entry #0 of PML4 to opint to PDPT[0] with present, writable, and user
    mov eax, 0x1007
    add eax, edi
    mov [edi], eax

    ; Do the same for PDPT
    add edi, 0x1000
    mov eax, 0x1003
    add eax, edi
    mov [edi], eax

    ; Map in 32 2MiB pages to map 64MiB of low memory
    add edi, 0x1000
    mov ebx, 0x87
    mov ecx, 32

.MintLoaderPagingSetEntryLoop:
    mov [edi], ebx
    add ebx, 0x200000
    add edi, 8
    loop .MintLoaderPagingSetEntryLoop

    ; Move CR3 to our PML4
    mov edi, MintLoaderBasePML
    mov cr3, edi

    ; Enable PAE
    mov eax, cr4
    or eax, 32
    mov cr4, eax

    ; Enable EFER
    mov ecx, 0xC0000080
    rdmsr
    or eax, 256
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Load our GDT
    lgdt [MintLoaderBaseGdtr]

    ; Leap of faith
    jmp 0x8:MintLoaderEntry64

    ; Else, failed, halt the system.
    jmp 0xFFFF:0
    cli
    hlt
    jmp $

; ========= 64-bit CODE START =========

[bits 64]
align 8

extern MintLoaderMain

MintLoaderEntry64:
    cli
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Call into the main loader code
    pop rcx             ; RCX = Multiboot info
    pop rdx             ; RDX = Magic

    and rsp, 0xFFFFFFFFFFFFFFF0
    mov rbp, rsp

    call MintLoaderMain

    ; We returned, disable IRQs + halt
    cli

_MintLoaderHang:
    hlt
    jmp _MintLoaderHang
