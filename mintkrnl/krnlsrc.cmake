# Is there a better way
# Please make it stop

include_directories(
    ${MINT_SOURCE_DIR}
    ${MINT_SOURCE_DIR}/mint/include/
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include/internal
    include
)

add_definitions(
    -D_MINTKRNL_
    -D_NTSYSTEM_
)

# ======= KI MODULE ======= 

if (ARCH STREQUAL "i386")
    list(APPEND KI_ARCH_SOURCE
            ki/arch/i386/ki_init.c)
endif()

list(APPEND KI_SOURCE ${KI_ARCH_SOURCE})


# ======= ASM =======



# ====== FINAL ======

list(APPEND SOURCE ${KI_SOURCE})
