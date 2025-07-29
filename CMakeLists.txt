# Main CMake build file for MiNT

cmake_minimum_required(VERSION 3.15)

project(MiNT  VERSION 1.0
		DESCRIPTION "The MiNT operating system"
		LANGUAGES C ASM)

# Enable NASM
enable_language(ASM_NASM)

# Set the CMake module path
set(MINT_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${MINT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_BINARY_DIR "${PROJECT_BINARY_DIR}")
set(MINT_BINARY_DIR "${PROJECT_BINARY_DIR}")
set(MINT_UTILITY_DIR "${MINT_SOURCE_DIR}/util/")

# Get project architecture
if (NOT ARCH)
	message(FATAL_ERROR "Please specify ARCH when building (available options: x86_64)")
endif()

string(TOLOWER ${ARCH} ARCH)

# Include our CMake scripts
include(cmake/cross_compile.cmake)
include(cmake/macros.cmake)
include(cmake/iso.cmake)

# Load the architecture profile
MINT_LOAD_PROFILE(${ARCH})

# Add C definitions
add_definitions(
	-D__MINT__
	-D__MINT_VERSION__=${CMAKE_PROJECT_VERSION}
	-D__MINT_VERSION_STR__="${CMAKE_PROJECT_VERSION}"
)

# Select the build type
if (NOT CMAKE_BUILDTYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(WARNING "No build type specified, using DEBUG build type")
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Select build type" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building for a DEBUG target")
    add_definitions(
        -D__DEBUG__
        -D__BUILDTYPE__=DEBUG
    )
else()
    message(STATUS "Building for a RELEASE target")
    add_definitions(
        -D__RELEASE__
        -D__BUILDTYPE__=RELEASE
    )
endif()

if (NOT CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR "You forgot to specify -DCMAKE_TOOLCHAIN_FILE. It should be cmake/arch/${ARCH}/toolchain.cmake")
endif() 

# Create our include directories
include_directories(
    include/libraries/libc/
    include/mint/
    include/
)

# More CMake flags
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# Create ISO target
set(MINT_ISO_DIR ${MINT_BINARY_DIR}/iso)
file(REMOVE_RECURSE ${MINT_ISO_DIR})

add_custom_target(iso ALL
                    COMMAND python3 ${MINT_UTILITY_DIR}/generate_iso.py ${MINT_BINARY_DIR}/boot/iso.$<CONFIG>.lst ${MINT_ISO_DIR}
                    COMMAND grub-mkrescue -o ${MINT_BINARY_DIR}/mint.iso ${MINT_BINARY_DIR}/iso
                    )

set_property(TARGET iso PROPERTY ALWAYS_RECOMPILE TRUE)

# Subdirectories
add_subdirectory("lib")
add_subdirectory("dll")
add_subdirectory("mintkrnl")
add_subdirectory("boot")

# Create ISO files
create_iso_list()