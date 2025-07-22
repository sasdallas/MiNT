#!/bin/sh

rm -rf build || true
mkdir build
cmake -S . -B build -DARCH=x86_64 -DCMAKE_TOOLCHAIN_FILE=cmake/arch/x86_64/toolchain.cmake
