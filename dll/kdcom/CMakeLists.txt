include("${CMAKE_MODULE_PATH}/CMakeMacros.cmake")
include("${CMAKE_MODULE_PATH}/cds.cmake")


list(APPEND KDCOM_SOURCE
        kdcom.c)

add_library(kdcom SHARED 
    ${KDCOM_SOURCE})

setModuleType(kdcom kerneldll ENTRYPOINT 0)
target_link_libraries(kdcom hal)

add_cd_file(TARGET kdcom FILE ${CMAKE_CURRENT_BINARY_DIR}/kdcom.dll DESTINATION MINT FOR bootcd) 