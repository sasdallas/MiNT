# Main CMake build file for MiNT

cmake_minimum_required(VERSION 3.15)

project(MINT VERSION 1.0
            DESCRIPTION "The MiNT operating system"
            LANGUAGES C ASM)

# CMake variables
set(CMAKE_MODULE_PATH "${MINT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


# MiNT directories
set(MINT_TOOLS_FOLDER "${CMAKE_CURRENT_BINARY_DIR}")


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
include(cmake/build_config.cmake)   # Configuration options
include(cmake/cross_compile.cmake)  # Cross-compilation (future)
include(cmake/cds.cmake)            # CD files

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

# Cross-compiling means we set our default CMake compiler to an i686 one
# Host tools need to be built with the host compiler.
if (NOT CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR "how are you not cross compiling")
else () 
    # Setup host tools
    setup_crosscompile()
endif()

# "Include" directories
add_subdirectory("mint/include/asm/")

# Setup project directories
add_subdirectory("boot")


# Create CD files
create_cd_lists()