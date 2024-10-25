# The MiNT operating system
An NT-like operating system written with a goal of understanding Windows' architecture.

In development and an experiment. Not designed to be commercial.

# Building
Building MiNT is best done via the RosBE environment for building ReactOS, as it has MinGW compilers.\
For reference, here are the versions I recommend:
- i686-w64-mingw32-gcc 8.4.0
- i686-w64-mingw32-binutils 2.34

Once you have that, make sure you have `syslinux-tools`, `cmake`, and are in the environment, and run `./configure.sh` to configure according to the architecture specified in the script.

Run `./build.sh` to build the OS, and the file `build/bootcd.iso` will be the MiNT BootCD.

# Licensing
While MiNT is in itself BSD licensed, some parts are not.

Files will always have their original sources and copyright disclaimers marked at the top.

If these files **DO NOT** have a copyright disclaimer at the top, you are free to use them in any way you see fit.

To clarify, files that DO NOT have a copyright disclaimer are released into the public domain.\
They come under NO warranty and are given in the hopes they are helpful.

If files are sourced from other projects, you should always be able to find their licenses in `EXTERNAL/`
