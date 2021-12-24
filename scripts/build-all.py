import os
import sys

def main(argv):
    for f in os.listdir(argv[1]):
        if f.endswith(".ilu"):
            print(f"Compiling {f}:")
            if os.system(f"./ilu {argv[1]}{f}") != 0: exit(1)
            print(f"Running {f}:")
            if os.system(f"time ./out") != 0: exit(1)
    os.system("./clean.sh")

if __name__ == "__main__":
    if os.system("./build.sh") != 0: exit(1)
    main(sys.argv)
