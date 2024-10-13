# configure.sh - Configure MiNT project to build

rm -rf build || true
mkdir build 
cmake -S . -B build -DARCH=i386 -DCMAKE_TOOLCHAIN_FILE=cmake/arch/i386/toolchain-i386.cmake