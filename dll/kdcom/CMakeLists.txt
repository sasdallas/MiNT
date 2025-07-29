include ("${CMAKE_MODULE_PATH}/macros.cmake")

add_definitions(-D_KDCOM_
                -D_KDDLL_)


list(APPEND KDCOM_SOURCES       kdcom.c)

include_directories(
    include
    ${MINT_SOURCE_DIR}/mintkrnl/include
)

add_library(kdcom SHARED ${KDCOM_SOURCES})
target_link_libraries(kdcom serial)
target_link_libraries(kdcom libc)
add_iso_file(TARGET kdcom FILE ${CMAKE_CURRENT_BINARY_DIR}/kdcom.dll DESTINATION root FOR iso)