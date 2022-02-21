#!/usr/bin/env python3
import os
import sys

def main(argv):
    for f in os.listdir(argv[1]):
        if f.endswith(".ach"):
            print(f"Compiling {f}:")
            if os.system(f"./ach {argv[1]}{f}") != 0: exit(1)
            print(f"Running {f}:")
            if os.system(f"time ./out") != 0: exit(1)
    os.system("make clean")

if __name__ == "__main__":
    if os.system("make -j$(nproc)") != 0: exit(1)
    main(sys.argv)
