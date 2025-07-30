# MiNTLDR sources

include_directories(
    ${MINT_SOURCE_DIR}
    ${MINT_SOURCE_DIR}/include/mint/
    ${CMAKE_CURRENT_BINARY_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

add_definitions(
    -D__MINTLDR__
)

# ===== ARCH SOURCES =====

if (ARCH STREQUAL "x86_64")
    list(APPEND __ARCH_SOURCES_ASM arch/x86_64/bootstub.asm)
    list(APPEND __ARCH_SOURCES_C 
                                arch/x86_64/main.c
                                arch/x86_64/graphics.c
                                arch/x86_64/serial.c
                                arch/x86_64/io.c
                                arch/x86_64/mm.c
                                arch/x86_64/ldr.c)

    set(__MINTLDR_LD_SCRIPT arch/x86_64/link.ld)
else()
    message(FATAL_ERROR "arch sources not defined in MiNTLDR")
endif()

list(APPEND __ARCH_SOURCES ${__ARCH_SOURCES_ASM} ${__ARCH_SOURCES_C})


# ===== UI SOURCES =====

list(APPEND __UI_SOURCES
                    ui/ui.c
                    ui/bugcheck.c)

# ===== MM SOURCES =====

list(APPEND __MM_SOURCES
                    mm/pmm.c
                    mm/vmm.c
                    mm/heap.c)

# ===== MINILIBC SOURCES =====

list(APPEND __MINILIBC_SOURCES
                    minilibc/string.c
                    minilibc/stdlib.c)

# ===== LDR SOURCES =====

list(APPEND __LDR_SOURCES
                    ldr/ldr.c
                    ldr/parameter.c)

# ===== FINAL SOURCES =====
list(APPEND __MINTLDR_SOURCE ${__ARCH_SOURCES} ${__UI_SOURCES} ${__MM_SOURCES} ${__LDR_SOURCES} ${__MINILIBC_SOURCES})