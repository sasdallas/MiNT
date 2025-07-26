#!/usr/bin/python3

# generate_iso.py: Takes in a list of ISO files and an output ISO directory to generate the output directory
import sys
import os
import shutil

if len(sys.argv) < 3:
    print("Usage: generate_iso.py [ISO catalogue] [ISO output directory]")
    sys.exit(1)

CATALOGUE = open(sys.argv[1], "r")
OUTPUT_DIR = sys.argv[2]

if len(OUTPUT_DIR) == 0:
    print("!!! No output directory specified. Not using your root!")
    sys.exit(1)

files = [line.strip() for line in CATALOGUE.readlines()]

for file in files:
    file_pair = file.split("=")
    full_file_path = OUTPUT_DIR + file_pair[0]
    print("==> Copying " + file_pair[1] + " to " + full_file_path)

    # Try to make the directory
    try:
        os.makedirs(os.path.dirname(full_file_path))
    except FileExistsError:
        pass

    # Copy the file
    shutil.copyfile(file_pair[1], full_file_path)

# !!!: Temporary GRUB
print("==> Generating boot/grub/grub.cfg")

try:
    os.makedirs(OUTPUT_DIR + "/boot/grub/")
except FileExistsError:
    pass

with open(OUTPUT_DIR + "/boot/grub/grub.cfg", "w+") as grub_config:
    grub_config.write("menuentry \"MiNT\" {\n\tmultiboot /mintldr.sys\n\tmodule /mintkrnl.exe kernel\n\tmodule /hal.dll hal.dll\n}")
    grub_config.close()

