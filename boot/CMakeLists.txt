PROJECT(MINTLDR)

include(mintldr_sources.cmake)



# Create a new executable

# Enable 32-bit code using some god-awful hackery
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -fwin32 <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>")
add_compile_options("$<$<NOT:$<COMPILE_LANGUAGE:ASM_NASM>>:-m32>")

add_executable(mintldr_executable ${__MINTLDR_SOURCE})
set_target_properties(mintldr_executable PROPERTIES ENABLE_EXPORTS TRUE DEFINE_SYMBOL "")

target_link_options(mintldr_executable PRIVATE "-m32")

# Linker 
get_filename_component(__LD_ABSOLUTE_PATH ${__MINTLDR_LD_SCRIPT} ABSOLUTE)
target_link_options(mintldr_executable PRIVATE "-Wl,-T,${__LD_ABSOLUTE_PATH}")
set_property(TARGET mintldr_executable APPEND PROPERTY LINK_DEPENDS ${__LD_ABSOLUTE_PATH})

# Set image base
set_image_base(mintldr_executable 0x100000)
set_image_subsystem(mintldr_executable native)
set_image_entrypoint(mintldr_executable MintLoaderEntry)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:mintldr_executable> ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:mintldr_executable>
)
add_custom_target(mintldr ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys)
