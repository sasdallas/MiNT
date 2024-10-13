enableA20_KKbrd_Out:
    cli                     // Clear interrupts
    pusha
    
    call waitInput          
    mov al, 0xAD            // Disable keyboard command
    out 0x64, al            // Disable the keyboard
    call waitInput          

    mov al, 0xD0            // Read output port command
    out 0x64, al            // Send read output port command to controller
    call waitOutput         // Wait for output

    in al, 0x60              
    push eax                // Get the output port data and store it
    call waitInput
    
    mov al, 0xD1            // Write output port command
    out 0x64, al            // Send write output port command to controller
    call waitInput          

    pop eax
    or al, 2                // Set bit 1(enable A20)
    out 0x60, al            // Write data back to the output port

    call waitInput          
    mov al, 0xAE            // Enable keyboard command
    out 0x64, al            // Enable keyboard

    call waitInput          

    popa
    sti                     // Enable interrupts
    ret                     // Return

waitInput:
    in al, 0x64
    test al, 2
    jnz waitInput
    ret