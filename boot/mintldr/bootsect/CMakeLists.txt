include(${CMAKE_MODULE_PATH}/bootsect.cmake)
include_directories(${MINT_SOURCE_DIR}/boot/mintldr)

if (ARCH STREQUAL "i386" OR ARCH STREQUAL "amd64")
    GenerateBootSector(fat  ${CMAKE_CURRENT_SOURCE_DIR}/fat.asm ${CMAKE_CURRENT_BINARY_DIR}/fat.bin)
    
else()
    message(FATAL_ERROR "Cannot create boot sector target for unknown architecture ${ARCH}")
endif()

