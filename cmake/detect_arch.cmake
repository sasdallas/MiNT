# Detecting host architecture

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