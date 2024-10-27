# Minbuild
A build environment for testing the MiNT kernel without a working bootloader.

## What?
Minbuild is an environment allowing the MINTKRNL to be tested without a working bootloader. It relies on hacking together a BootCD image from the ReactOS project.

## How?
To use Minbuild, you must do the following steps:
1. Clone ReactOS to this folder
2. Using RosBE (which you should use for MiNT anyways), build ReactOS to an I386 architecture.
3. Take note of the folder. Use the command `export MINBUILD_FOLDER=<build folder>` to add the env variable `$MINBUILD_FOLDER`.
4. Run `./build.sh minbuild` every time you want to build the MiNT kernel.
5. A new BootCD image known as `minbuild_bootcd.iso` will be copied to the build/ directory.

## WARNING
MiNT does not provide all the HAL exports, KDCOM exports, NTOS exports, etc. that ReactOS does.\
When booting, you will encounter a LOT of failed to load boot-time driver messages. Just spam OK and continue.

## Can you please stop stealing from ReactOS?
Never!