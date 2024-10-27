include("${CMAKE_MODULE_PATH}/CMakeMacros.cmake")
include("${CMAKE_MODULE_PATH}/cds.cmake")


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

list(APPEND KDCOM_SOURCE
        kdcom.c)



add_library(kdcom SHARED 
    ${KDCOM_SOURCE})

setModuleType(kdcom kerneldll ENTRYPOINT 0)
target_link_libraries(kdcom hal)
target_link_libraries(kdcom cportlib)

add_cd_file(TARGET kdcom FILE ${CMAKE_CURRENT_BINARY_DIR}/kdcom.dll DESTINATION MINT FOR bootcd) 