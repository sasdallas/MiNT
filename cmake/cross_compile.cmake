# Handles cross compilation

include(ExternalProject)

function(detect_host_architecture) 
    # ARCH is all lowercase, lowercase CMAKE_HOST_SYSTEM_PROCESSOR too
    string(TOLOWER "${CMAKE_HOST_SYSTEM_PROCESSOR}" lowercase_HOST_SYSTEM_PROC)
    if (lowercase_HOST_SYSTEM_PROC STREQUAL x86 OR lowercase_HOST_SYSTEM_PROC MATCHES "^[3456]86$")
        set(HOST_ARCH i386)
    elseif (lowercase_HOST_SYSTEM_PROC STREQUAL x86_64 OR lowercase_HOST_SYSTEM_PROC STREQUAL "amd64")
        set(HOST_ARCH x86_64)
    else()
        message(FATAL_ERROR "Unsupported/unknown host architecture: ${lowercase_HOST_SYSTEM_PROC}")
    endif()
endfunction()

function(setup_crosscompile)
    list(APPEND HOST_TOOLS obj2bin)

    # See root project CMake file for explanation
    ExternalProject_add(tools
        PREFIX tools
        SOURCE_DIR ${MINT_SOURCE_DIR}/tools
        CMAKE_ARGS
            -UCMAKE_TOOLCHAIN_FILE
            -DARCH:STRING=${ARCH}
            -DCMAKE_INSTALL_PREFIX=${MINT_BINARY_DIR}/tools
            -DMINT_SOURCE_DIR=${MINT_SOURCE_DIR}
            -DMINT_TOOLS_FOLDER=${MINT_BINARY_DIR}/tools/bin
            -DTARGET_COMPILER_ID=${CMAKE_C_COMPILER_ID}
        INSTALL_COMMAND ${CMAKE_COMMAND} -E true)
    
    ExternalProject_Get_Property(tools INSTALL_DIR)

    foreach(_tool ${HOST_TOOLS})
        add_executable(mint-${_tool} IMPORTED)
        set_target_properties(mint-${_tool} PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/bin/${_tool})
        add_dependencies(mint-${_tool} tools ${INSTALL_DIR}/bin/${_tool})
    endforeach()
endfunction()