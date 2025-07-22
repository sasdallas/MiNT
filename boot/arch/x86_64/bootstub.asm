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
