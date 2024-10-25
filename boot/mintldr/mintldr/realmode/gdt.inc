
installGDT:
    cli                     // Clear interrupts
    pusha                   // Save the registers
    lgdt [toc]              // Load GDT into GDTR
    sti                     // Re-enable interrupts
    popa                    // Restore registers
    ret                     // Return!


// ----------------------------------------
// Global Descriptor Table data
// ----------------------------------------

gdt_data:
    // Null segment
    .word HEX(0000)
    .word HEX(0000)
    .word HEX(0000)
    .word HEX(0000)
    
    // 32-bit GDT code segment
    .word HEX(FFFF)            // Limit low
    .word 0                    // Base low
    .byte 0                    // Base middle
    .byte BIN(10011010)        // Access
    .byte BIN(11001111)        // Granularity
    .byte 0                    // Base high

    // 32-bit GDT data segment
    .word HEX(FFFF)            // Limit low, again.
    .word 0                    // Base low
    .word HEX(9200)
    .word HEX(00CF)

    // 16-bit GDT code segment
    .word HEX(FFFF)
    .word HEX(0000)
    .word HEX(9E00)
    .word HEX(0000)

    // 16-bit GDT data segment
    .word HEX(FFFF)
    .word HEX(0000)
    .word HEX(9200)
    .word HEX(0000)


gdt_end:
toc:
    .word gdt_end - gdt_data - 1
    .long gdt_data             // Base of GDT

// Descriptor offsets names

#define NULL_DESC 0
#define CODE_DESC 0x8
#define DATA_DESC 0x10