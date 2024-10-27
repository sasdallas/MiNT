include("${CMAKE_MODULE_PATH}/CMakeMacros.cmake")
include("${CMAKE_MODULE_PATH}/cds.cmake")


list(APPEND KDCOM_SOURCE
        kdcom.c)

# SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "")


add_library(kdcom SHARED 
    ${KDCOM_SOURCE})

setModuleType(kdcom kerneldll ENTRYPOINT 0)
target_link_libraries(kdcom hal)
