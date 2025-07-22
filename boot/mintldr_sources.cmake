# MiNTLDR sources

include_directories(
    ${MINT_SOURCE_DIR}
    ${MINT_SOURCE_DIR}/include/mint/
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

add_definitions(
    -D__MINTLDR__
)

# ===== ARCH SOURCES =====

if (ARCH STREQUAL "x86_64")
    list(APPEND __ARCH_SOURCES_ASM arch/x86_64/bootstub.asm)
    list(APPEND __ARCH_SOURCES_C arch/x86_64/main.c)
    set(__MINTLDR_LD_SCRIPT arch/x86_64/link.ld)
else()
    message(FATAL_ERROR "arch sources not defined in MiNTLDR")
endif()

list(APPEND __ARCH_SOURCES ${__ARCH_SOURCES_ASM} ${__ARCH_SOURCES_C})

# ===== FINAL SOURCES =====
list(APPEND __MINTLDR_SOURCE ${__ARCH_SOURCES})