
include_directories(
    ${MINT_SOURCE_DIR}
    ${MINT_SOURCE_DIR}/include/mint/
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

add_definitions(
    -D__MINTKRNL__
    -D_NTSYSTEM_
)

# ======= KI MODULE ========

if (ARCH STREQUAL "x86_64")
    list(APPEND __KI_ARCH_SOURCE
                ki/arch/x86_64/init.c)
else()
    message(FATAL_ERROR "ki module not defined for architecture: ${ARCH}")
endif()

list(APPEND __KI_SOURCE ${__KI_ARCH_SOURCE})

# ======= FINAL SOURCES ========

list(APPEND __MINTKRNL_SOURCE ${__KI_SOURCE})