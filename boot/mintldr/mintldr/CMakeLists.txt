# MINTLDR CMake file

include("${CMAKE_MODULE_PATH}/CMakeMacros.cmake")
include("${CMAKE_MODULE_PATH}/cds.cmake")
include("${CMAKE_MODULE_PATH}/bootsect.cmake")

# Setup include parameters
include_directories(BEFORE include)
include_directories(
    include/
    ${MINT_SOURCE_DIR}/mint/include/asm/
    ${MINT_SOURCE_DIR}/mint/include/mint/
)

# Create boot sector target
GenerateBootSectorGAS(mintldr_realmode
                ${CMAKE_CURRENT_SOURCE_DIR}/realmode/i386.S
                ${CMAKE_CURRENT_BINARY_DIR}/mintldr_realmode.bin
                F800)
                

# Assemble general sources
include("mintldr.cmake")

add_library(
    mintldr_general
    ${MINTLDR_SOURCE}
)

# add_dependencies(mintldr_general xdk)

# Create the general MINTLDR library, which will be linked to mintldr_executable


if (ARCH STREQUAL "i386")
    # Building for an i386 target
    list(APPEND MINTLDR_ARCH_ASM_SOURCE
            arch/i386/entry.S
            arch/i386/idt.S)

    list(APPEND MINTLDR_ARCH_C_SOURCE
            arch/i386/mach/mach.c
            arch/i386/idt.c
            arch/i386/irq.c
            arch/i386/gfx.c
            arch/i386/tinyhal.c)
else()
    message(FATAL_ERROR "No targets besides i386 have been implemented")
endif()


# Concatenate list of sources together
list(APPEND MINTLDR_ARCH_SOURCE
        ${MINTLDR_ARCH_ASM_SOURCE}
        ${MINTLDR_ARCH_C_SOURCE})

# Create a new executable
add_executable(mintldr_executable ${MINTLDR_ARCH_SOURCE})
set_target_properties(mintldr_executable PROPERTIES ENABLE_EXPORTS TRUE DEFINE_SYMBOL "")

# Setup executable properties
target_link_options(mintldr_executable PRIVATE -Wl,--exclude-all-symbols,--file-alignment,0x200,--section-alignment,0x200)

get_filename_component(_linker_script "mintldr.ld" ABSOLUTE)
target_link_options(mintldr_executable PRIVATE "-Wl,-T,${_linker_script}")
set_property(TARGET mintldr_executable APPEND PROPERTY LINK_DEPENDS ${_linker_script})

add_custom_command(TARGET mintldr_executable
                    POST_BUILD
                    COMMAND ${CMAKE_STRIP} --remove-section=.rossym $<TARGET_FILE:mintldr_executable>
                    COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:mintldr_executable>)

# Setup dependencies
add_dependencies(mintldr_executable asm)


# Setup parameters for that executable
setImageBase(mintldr_executable 0x10000) # Higher half baby
setImageSubsystem(mintldr_executable native)
setEntrypoint(mintldr_executable arch_entrypoint)

# Add in our mintldr common
target_link_libraries(mintldr_executable mintldr_general minilib_library)

# This will concatenate the realmode bootsector and mintldr together into MINTLDR.SYS
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys
    COMMAND cat 
    ${CMAKE_CURRENT_BINARY_DIR}/mintldr_realmode.bin
        ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:mintldr_executable>
         > ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:mintldr_executable> ${CMAKE_CURRENT_BINARY_DIR}/mintldr_realmode.bin
)

add_custom_target(mintldr ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys)


# Add the file in
add_cd_file(TARGET mintldr FILE ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys DESTINATION loader FOR bootcd) 
