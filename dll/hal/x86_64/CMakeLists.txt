include ("${CMAKE_MODULE_PATH}/macros.cmake")

add_definitions(-D_MINTHAL_
                -D_MINTHALDLL_)


include_directories(
    include
    ${MINT_SOURCE_DIR}/mintkrnl/include
)

# Include HAL source files
include("hal_sources.cmake")
set(HAL_SOURCE ${__HAL_SOURCE})

# Create the HAL
add_library(hal SHARED ${HAL_SOURCE})

add_iso_file(TARGET hal FILE ${CMAKE_CURRENT_BINARY_DIR}/hal.dll DESTINATION root FOR iso) 