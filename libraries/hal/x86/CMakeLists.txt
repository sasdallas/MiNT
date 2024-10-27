include ("${CMAKE_MODULE_PATH}/CMakeMacros.cmake")

add_definitions(-D_MINTHAL_
                -D_MINTHALDLL_)


include_directories(
    include
    ${MINT_SOURCE_DIR}/mintkrnl/include
)

# Include HAL source files
include("halsource.cmake")

# Create the HAL
add_library(hal SHARED ${HAL_SOURCE})

# Link to MINTKRNL
#target_link_libraries(hal mintkrnl)

setModuleType(hal kerneldll ENTRYPOINT HalInitSystem)

add_cd_file(TARGET hal FILE ${CMAKE_CURRENT_BINARY_DIR}/hal.dll DESTINATION MINT FOR bootcd) 