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

# Includes
include(cmake/build_config.cmake)   # Configuration options
include(cmake/cross_compile.cmake)  # Cross-compilation (future)
include(cmake/cds.cmake)            # CD files
include(GenerateExportHeader)       # GenerateExportHeader (CMake default) 

# MiNT specific compiler flags
# __MINT__          Identifies MiNT project
# VERSION           Version number. Not a string!
# VERSION_STR       Version string
add_definitions(
    -D__MINT__
    -DVERSION=${CMAKE_PROJECT_VERSION}
    -DVERSION_STR="${CMAKE_PROJECT_VERSION}"
)

# Setup the operating system build configuration
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(WARNING "No build type specified, using DEBUG build type")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Select build type" FORCE)
endif()


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building for a DEBUG target")
    add_definitions(
        -D__DEBUG__
        -DBUILDTYPE=DEBUG
    )
else()
    message(STATUS "Building for a RELEASE target")
    add_definitions(
        -D__RELEASE__
        -DBUILDTYPE=RELEASE
    )
endif()

# A bit of extra compiler flags
add_definitions(
    -D_CRT_NON_CONFORMING_SWPRINTFS
)

# Fix for __FILE__ being absolute
file(RELATIVE_PATH _PATH_PREFIX ${MINT_BINARY_DIR} ${MINT_SOURCE_DIR})
add_compile_options(-ffile-prefix-map=${MINT_SOURCE_DIR}=)
add_compile_options(-ffile-prefix-map=${_PATH_PREFIX}=)

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

# Actual include directories
include_directories(
    mint/include/
    mint/include/mint/
    ${MINT_BINARY_DIR}/mint/include/
    ${MINT_BINARY_DIR}/mint/include/mint/
    ${MINT_BINARY_DIR}/mint/include/mint/ddk/
    ${MINT_BINARY_DIR}/mint/include/mint/psdk/
    ${MINT_BINARY_DIR}/mint/include/mint/xdk/
)

# Extra CMake flags
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)


# Setup project directories
add_subdirectory("boot")
add_subdirectory("dll")
add_subdirectory("mintkrnl")




# Create CD files
create_cd_lists()
