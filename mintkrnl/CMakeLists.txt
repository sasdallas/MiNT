PROJECT(MiNTKRNL)

include(mintkrnl_sources.cmake)

set(MINTKRNL_SOURCES ${__MINTKRNL_SOURCE})

add_executable(mintkrnl
    ${MINTKRNL_SOURCES}
    )


set_target_properties(
    mintkrnl
    PROPERTIES
    ENABLE_EXPORTS TRUE
    DEFINE_SYMBOL ""
)

target_link_options(mintkrnl
    PRIVATE -Wl,--major-image-version,5 -Wl,--minor-image-version,01 -Wl,--major-os-version,5 -Wl,--minor-os-version,01)

target_link_options(mintkrnl
    PRIVATE  -Wl,--exclude-all-symbols,--file-alignment=0x1000,-section-alignment=0x1000)

set_image_entrypoint(mintkrnl KiSystemStartup)
set_image_base(mintkrnl 0x00400000)
set_image_subsystem(mintkrnl native)

add_iso_file(TARGET mintkrnl FILE ${CMAKE_CURRENT_BINARY_DIR}/mintkrnl.exe DESTINATION root FOR iso)