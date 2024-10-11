# Main CMake build file for MiNT

cmake_minimum_required(VERSION 3.15)

project(MINT VERSION 1.0
            DESCRIPTION "The MiNT operating system"
            LANGUAGES C ASM)

# CMake variables
set(CMAKE_MODULE_PATH "${MINT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# ARCH must be defined.
if (NOT ARCH)
    message(FATAL_ERROR "Please use the -DARCH=<architecture> option with one of the following architectures: i386")
endif()

string(TOLOWER ${ARCH} ARCH)

# Setup the operating system build configuration
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Using DEBUG build type")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Select build type" FORCE)
endif()

# Includes
include(cmake/build_config.cmake) # Configuration options
include(cmake/detect_arch.cmake) # Cross-compilation (future)

# MiNT specific compiler flags
# __MINT__          Identifies MiNT project
# VERSION           Version number. Not a string!
# VERSION_STR       Version string
add_definitions(
    -D__MINT__
    -DVERSION=${CMAKE_PROJECT_VERSION}
    -DVERSION_STR="${CMAKE_PROJECT_VERSION}"
)

# A bit of extra compiler flags
add_definitions(
    -D_CRT_NON_CONFORMING_SWPRINTFS
)

# Languages


# Setup project directories
add_subdirectory("boot")
