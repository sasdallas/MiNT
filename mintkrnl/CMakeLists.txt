
PROJECT(MINTKRNL)

include(krnlsrc.cmake)
include("${CMAKE_MODULE_PATH}/CMakeMacros.cmake")
include("${CMAKE_MODULE_PATH}/cds.cmake")

set(MINTKRNL_SOURCE ${SOURCE})
set(MINTKRNL_ASM_SOURCE ${ASM_SOURCE})



add_executable(mintkrnl
    ${MINTKRNL_ASM_SOURCE}
    ${MINTKRNL_SOURCE}
    )

# Too scared to make this a setModuleType
setImageEntrypoint(mintkrnl KiSystemStartup)
setImageBase(mintkrnl 0x00400000)
setImageSubsystem(mintkrnl native)

set_target_properties(
    mintkrnl
    PROPERTIES
    ENABLE_EXPORTS TRUE
    DEFINE_SYMBOL ""
)




# Apply NT OS flags
target_link_options(mintkrnl
        PRIVATE -Wl,--major-image-version,5 -Wl,--minor-image-version,01 -Wl,--major-os-version,5 -Wl,--minor-os-version,01)

# Apply extra flags
target_link_options(mintkrnl PRIVATE -Wl,--exclude-all-symbols,--file-alignment=0x1000,-section-alignment=0x1000)

# Add in init-sections
get_filename_component(_linker_script ${MINT_SOURCE_DIR}/cmake/init-sections.lds ABSOLUTE)
target_link_options(mintkrnl PRIVATE "-Wl,-T,${_linker_script}")
set_property(TARGET mintkrnl APPEND PROPERTY LINK_DEPENDS ${_linker_script})

# Run PE fixup to mess with sections
# Imma be real I stole this entire build system because I have no idea what I'm doing
add_custom_command(
    TARGET mintkrnl POST_BUILD
    COMMAND mint-pefixup --kernel $<TARGET_FILE:mintkrnl> 
    DEPENDS mint-pefixup
)
#set_property(TARGET mintkrnl APPEND PROPERTY LINK_DEPENDS $<TARGET_PROPERTY:mint-pefixup,IMPORTED_LOCATION>)

# RTL, HAL, and other libraries will be made elsewhere
target_link_libraries(mintkrnl hal)
target_link_libraries(mintkrnl kdcom)


add_cd_file(TARGET mintkrnl FILE ${CMAKE_CURRENT_BINARY_DIR}/mintkrnl.exe DESTINATION MINT FOR bootcd) 

