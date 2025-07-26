# Hardware Abstraction Layer
project(HAL)

if ((ARCH STREQUAL "x86_64"))
    add_subdirectory("x86_64")
else()
    message(FATAL "Compiling HAL for unsupported architecture: ${ARCH}")
endif()