OUTPUT_FORMAT(pe-i386)
ENTRY(MintLoaderEntry)

SECTIONS
{
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)

        __text_start = .;
		*(.text)
        __text_end = .;
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
        __rodata_start = .;
		*(.rodata)
        __rodata_end = .;
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
        __data_start = .;
		*(.data)
        __data_end = .;
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
        __bss_start = .;
		*(COMMON)
		*(.bss)
        __bss_end = .;
	}

    /DISCARD/ :
    {
        *(*)
    }

}