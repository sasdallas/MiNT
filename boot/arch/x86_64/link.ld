SECTIONS
{

  .text __image_base__ + __section_alignment__ : ALIGN(__file_alignment__)
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