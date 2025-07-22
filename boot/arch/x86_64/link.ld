SECTIONS
{
  . = 1M;

  .text BLOCK(4K) : ALIGN(4K)
  {
	  *(.multiboot)

    __text_start = .;
	  *(.text)
    __text_end = .;

    __data_start = .;
    *(.data)
    *(SORT(.data*))
    *(.xdata)
    *(.pdata)
    *(.rdata)
    *(SORT(.rdata*))
    __data_end = .;
  }

  .edata BLOCK(__file_alignment__) :
  {
    *(.edata)
  }

  .bss :
  {
    __bss_start = . ;
    *(.bss)
    *(COMMON)
    __bss_end = . ;
  }


  /DISCARD/ :
  {
    *(*)
  }
}