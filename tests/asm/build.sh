set -xe

nasm -felf64 stuff.asm
ld -o stuff stuff.o

rm stuff.o
