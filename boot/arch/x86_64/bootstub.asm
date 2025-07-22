; MINTLDR stub for booting from Multiboot

; Multiboot header

MULTIBOOT_PAGE_ALIGN        equ 1 << 0
MULTIBOOT_MEMINFO           equ 1 << 1

MULTIBOOT_FLAGS             equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMINFO
MULTIBOOT_MAGIC             equ 0x1BADB002
MULTIBOOT_CHECKSUM          equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

section .multiboot
align 4
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

section .bss
align 16
MintLoaderStackBottom:
resb 16384
MintLoaderStackTop:

section .text

global MintLoaderEntry
extern _MintLoaderMain
MintLoaderEntry:
    mov esp, MintLoaderStackTop
    
    ; Call the MintLoaderMain function
    call _MintLoaderMain

    ; We returned, disable IRQs + halt
    cli

_MintLoaderHang:
    hlt
    jmp _MintLoaderHang
