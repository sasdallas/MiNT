

enableA20_KKbrd_Out:
    cli                     // Clear interrupts
    pusha
    
    call waitInput          
    
    mov al, HEX(0D1)
    out HEX(064), al
    call waitInput

    mov al, HEX(0DF)
    out HEX(060), al
    call waitInput

    mov al, HEX(0FF)
    out HEX(064), al
    call waitInput

    popa
    ret


waitInput:
    // Extra instructions to be encoded
    .word HEX(00eb), HEX(00eb) // jmp $+2, jmp $+22

    in al, 0x64
    cmp al, HEX(0FF)
    jz waitInput
    test al, 2
    jnz waitInput

    ret