# i386 toolchain cmake

set(CMAKE_SYSTEM_NAME       Windows)
set(CMAKE_SYSTEM_PROCESSOR  i686)

# Unless specified, use -lgcc
set(CMAKE_C_STANDARD_LIBRARIES "-lgcc" CACHE STRING "Standard C libraries")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lgcc" CACHE STRING "Standard C++ libraries")

# Compilers
set(CMAKE_C_COMPILER    i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER  i686-w64-mingw32-g++)
set(CMAKE_ASM_COMPILER  i686-w64-mingw32-gcc)
set(CMAKE_ASM_COMPILER_ID "GNU")
set(CMAKE_MC_COMPILER   i686-w64-mingw32-windmc)
set(CMAKE_RC_COMPILER   i686-w64-mingw32-windres)
set(CMAKE_OBJCOPY       i686-w64-mingw32-objcopy)

# CMake should only test these without linking (it will try to use Glibc)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Modify linker flags
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-nostdlib -Wl,--enable-auto-image-base,--disable-auto-import")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-nostdlib -Wl,--enable-auto-image-base,--disable-auto-import")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostdlib -Wl,--enable-auto-image-base,--disable-auto-import")


set(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32)

add_compile_options("$<$<NOT:$<COMPILE_LANGUAGE:CXX>>:-nostdinc>")

message (STATUS "Building for i686 toolchain with MinGW32-based compiler")

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)