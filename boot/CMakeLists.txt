PROJECT(MINTLDR)

include(mintldr_sources.cmake)

# Create a new executable
add_executable(mintldr_executable ${__MINTLDR_SOURCE})
set_target_properties(mintldr_executable PROPERTIES ENABLE_EXPORTS TRUE DEFINE_SYMBOL "")

# Linker script
# target_link_options(mintldr_executable PRIVATE -Wl,--exclude-all-symbols,--file-alignment,0x200,--section-alignment,0x200)
get_filename_component(__LD_ABSOLUTE_PATH ${__MINTLDR_LD_SCRIPT} ABSOLUTE)
target_link_options(mintldr_executable PRIVATE "-Wl,-T,${__LD_ABSOLUTE_PATH}")
set_property(TARGET mintldr_executable APPEND PROPERTY LINK_DEPENDS ${__LD_ABSOLUTE_PATH})

    target_link_options(mintldr_executable PRIVATE -Wl,--exclude-all-symbols,--file-alignment,0x200,--section-alignment,0x200)
    # Strip everything, including rossym data
    add_custom_command(TARGET mintldr_executable
                    POST_BUILD
                    COMMAND ${CMAKE_STRIP} --remove-section=.rossym $<TARGET_FILE:mintldr_executable>
                    COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:mintldr_executable>)

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

add_iso_file(TARGET mintldr FILE ${CMAKE_CURRENT_BINARY_DIR}/mintldr.sys DESTINATION root FOR iso)