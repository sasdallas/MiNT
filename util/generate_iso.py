#!/usr/bin/python3

# generate_iso.py: Takes in a list of ISO files and an output ISO directory to generate the output directory
import sys
import os

if len(sys.argv) < 3:
    print("Usage: generate_iso.py [ISO catalogue] [ISO output directory]")
    sys.exit(1)

CATALOGUE = open(sys.argv[1], "r")
OUTPUT_DIR = sys.argv[2]

files = CATALOGUE.readlines()

for file in files:
    file_pair = file.split("=")
    print("Copying " + file_pair[1] + " to " + file_pair[0])