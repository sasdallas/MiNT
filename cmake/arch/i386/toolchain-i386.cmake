# i386 toolchain cmake

set(CMAKE_SYSTEM_NAME       Windows)
set(CMAKE_SYSTEM_PROCESSOR  i686)

# Unless specified, use -lgcc
set(CMAKE_C_STANDARD_LIBRARIES "-lgcc" CACHE STRING "Standard C libraries")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lgcc" CACHE STRING "Standard C++ libraries")

# Compilers
set(CMAKE_C_COMPILER    i686-elf-gcc)
set(CMAKE_CXX_COMPILER  i686-elf-g++)

# CMake should only test these without linking (it will try to use Glibc)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Modify linker flags
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-nostdlib -Wl,--enable-auto-image-base,--disable-auto-import")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-nostdlib -Wl,--enable-auto-image-base,--disable-auto-import")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostdlib -Wl,--enable-auto-image-base,--disable-auto-import")


set(CMAKE_FIND_ROOT_PATH /usr/i686-elf)


