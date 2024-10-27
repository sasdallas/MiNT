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
add_library(hal SHARED ${hal_source})

# Link to MINTKRNL
#target_link_libraries(hal mintkrnl) 

setModuleType(hal kerneldll ENTRYPOINT HalInitSystem)
