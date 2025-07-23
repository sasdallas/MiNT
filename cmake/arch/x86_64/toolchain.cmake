# x86_64 toolchain

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# # Unless specified, use -lgcc
# set(CMAKE_C_STANDARD_LIBRARIES "-lgcc" CACHE STRING "Standard C libraries")
# set(CMAKE_CXX_STANDARD_LIBRARIES "-lgcc" CACHE STRING "Standard C++ libraries")

# Compilers
set(CMAKE_C_COMPILER    x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER  x86_64-w64-mingw32-g++)
set(CMAKE_ASM_COMPILER  x86_64-w64-mingw32-gcc)
set(CMAKE_ASM_COMPILER_ID "GNU")
set(CMAKE_MC_COMPILER   x86_64-w64-mingw32-windmc)
set(CMAKE_RC_COMPILER   x86_64-w64-mingw32-windres)
set(CMAKE_OBJCOPY       x86_64-w64-mingw32-objcopy)

set(CMAKE_C_STANDARD_LIBRARIES "")
set(CMAKE_CXX_STANDARRD_LIBRARIES "")
set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")

# CMake should only test these without linking (it will try to use Glibc)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Modify linker flags
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-nostdlib -nodefaultlibs -Wl,--enable-auto-image-base,--disable-auto-import")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-nostdlib -nodefaultlibs -Wl,--enable-auto-image-base,--disable-auto-import")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostdlib -nodefaultlibs -Wl,--enable-auto-image-base,--disable-auto-import")

add_compile_options("$<$<NOT:$<COMPILE_LANGUAGE:ASM_NASM>>:-Wno-builtin-declaration-mismatch>")

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER>  -x assembler-with-cpp -o <OBJECT> <INCLUDES> <FLAGS> <DEFINES> -D__ASM__ -c <SOURCE>")

# CMake tripping

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

add_compile_options("$<$<NOT:$<COMPILE_LANGUAGE:ASM_NASM>>:-nostdinc>")

message (STATUS "Building for x86_64 toolchain with MinGW32-based compiler")

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)