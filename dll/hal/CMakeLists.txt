# Hardware Abstraction Layer
project(HAL)

if ((ARCH STREQUAL "i386"))
    add_subdirectory("x86")
else()
    message(FATAL "Compiling HAL for unsupported architecture: ${ARCH}")
endif()