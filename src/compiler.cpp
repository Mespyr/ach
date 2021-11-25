#include "../include/compiler.h"
#include <iostream>

void compile_to_asm(std::vector<Op> program, std::string output_filename)
{
    File outfile(output_filename);

    outfile.write("section .text\n");
    outfile.write("global _start\n");
    outfile.write("_start:\n");

    for (Op op : program) {}

    outfile.write("\tmov rax, 60\n");
    outfile.write("\tmov rdi, 0\n");
    outfile.write("\tsyscall\n");
}
