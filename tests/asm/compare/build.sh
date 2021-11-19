set -xe

nasm -felf64 syntax.asm
ld -o syntax syntax.o

rm syntax.o
