#include <arch/i386/i386.h>
#include <reactos/x86common.h>

RealModeBios32:
    /* Save registers + segment registers */
    push ds
    push es
    push fs 
    push gs
    pushad

    /* Get the interrupt vector & patch opcode */
    mov al, byte ptr ds:[BSS_IntVector]
    mov byte ptr ds:[RealModeBios32_Interrupt], al

    /* Mask ZF/CF/SF in EFLAGS by pushing registers, popping CX, and masking */
    pushf 
    pop cx
    and cx, not (EFLAGS_CF or EFLAGS_ZF or EFLAGS_SF)

    /* Get flags CF, ZF, and SF from structure + combine them in EFLAGS */
    mov ax, word ptr cs:[BSS_RegisterSet + REGS_EFLAGS]
    and ax, (EFLAGS_CF or EFLAGS_ZF or EFLAGS_SF)
    or ax, cx
    push ax
    popf

    /* Setup our segment selectors */
    mov ax, word ptr cs:[BSS_RegisterSet + REGS_DS]
    mov ds, ax
    mov ax, word ptr cs:[BSS_RegisterSet + REGS_ES]
    mov es, ax
    mov ax, word ptr cs:[BSS_RegisterSet + REGS_FS]
    mov fs, ax
    mov ax, word ptr cs:[BSS_RegisterSet + REGS_GS]
    mov gs, ax
    
    /* Setup main registers */
    mov eax, dword ptr cs:[BSS_RegisterSet + REGS_EAX]
    mov ebx, dword ptr cs:[BSS_RegisterSet + REGS_EBX]
    mov ecx, dword ptr cs:[BSS_RegisterSet + REGS_ECX]
    mov edx, dword ptr cs:[BSS_RegisterSet + REGS_EDX]
    mov esi, dword ptr cs:[BSS_RegisterSet + REGS_ESI]
    mov edi, dword ptr cs:[BSS_RegisterSet + REGS_EDI]
    mov ebp, dword ptr cs:[BSS_RegisterSet + REGS_EBP]

    /* Enncode interrupt vector (CD = INT opcode) */
    .byte HEX(0cd)
RealModeBios32_Interrupt:
    .byte 00

    /* Ah, all done! Time to save registers */
    mov dword ptr cs:[BSS_RegisterSet + REGS_EAX], eax
    mov dword ptr cs:[BSS_RegisterSet + REGS_EBX], ebx
    mov dword ptr cs:[BSS_RegisterSet + REGS_ECX], ecx
    mov dword ptr cs:[BSS_RegisterSet + REGS_EDX], edx
    mov dword ptr cs:[BSS_RegisterSet + REGS_ESI], esi
    mov dword ptr cs:[BSS_RegisterSet + REGS_EDI], edi
    mov dword ptr cs:[BSS_RegisterSet + REGS_EBP], ebp

    mov ax, ds
    mov word ptr cs:[BSS_RegisterSet + REGS_DS], ax
    mov es, ax
    mov word ptr cs:[BSS_RegisterSet + REGS_ES], ax
    mov fs, ax
    mov word ptr cs:[BSS_RegisterSet + REGS_FS], ax
    mov gs, ax
    mov word ptr cs:[BSS_RegisterSet + REGS_GS], ax

    pushfd
    pop dword ptr cs:[BSS_RegisterSet + REGS_EFLAGS]

    /* Restore & return */
    popad
    pop gs
    pop fs
    pop es
    pop ds

    ret
    