# The MiNT operating system

In progress and unfinalized.

## Warning

MiNT is a learning project. If you want to see a *proper* Windows clone, please see the ReactOS repository.

## Building

`cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/arch/x86_64/toolchain.cmake -DARCH=x86_64`