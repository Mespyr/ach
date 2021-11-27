#include "../include/compiler.h"

void compile_to_asm(std::vector<Op> program, std::string output_filename)
{
    File outfile(output_filename, MODE_WRITE);
    
    // write boilerplate into file
    outfile.writeln("section .text");
    outfile.writeln("global _start");
 
    outfile.writeln("print:");
    outfile.writeln("\tmov r9, -3689348814741910323");
    outfile.writeln("\tsub rsp, 40");
    outfile.writeln("\tmov BYTE [rsp+31], 10");
    outfile.writeln("\tlea rcx, [rsp+30]");
    outfile.writeln(".L2:");
    outfile.writeln("\tmov rax, rdi");
    outfile.writeln("\tlea r8, [rsp+32]");
    outfile.writeln("\tmul r9");
    outfile.writeln("\tmov rax, rdi");
    outfile.writeln("\tsub r8, rcx");
    outfile.writeln("\tshr rdx, 3");
    outfile.writeln("\tlea rsi, [rdx+rdx*4]");
    outfile.writeln("\tadd rsi, rsi");
    outfile.writeln("\tsub rax, rsi");
    outfile.writeln("\tadd eax, 48");
    outfile.writeln("\tmov BYTE [rcx], al");
    outfile.writeln("\tmov rax, rdi");
    outfile.writeln("\tmov rdi, rdx");
    outfile.writeln("\tmov rdx, rcx");
    outfile.writeln("\tsub rcx, 1");
    outfile.writeln("\tcmp rax, 9");
    outfile.writeln("\tja .L2");
    outfile.writeln("\tlea rax, [rsp+32]");
    outfile.writeln("\tmov edi, 1");
    outfile.writeln("\tsub rdx, rax");
    outfile.writeln("\txor eax, eax");
    outfile.writeln("\tlea rsi, [rsp+32+rdx]");
    outfile.writeln("\tmov rdx, r8");
    outfile.writeln("\tmov rax, 1");
    outfile.writeln("\tsyscall");
    outfile.writeln("\tadd rsp, 40");
    outfile.writeln("\tret");

    outfile.writeln("_start:");

    for (Op op : program) 
    {
        if (op.type == OP_PUSH)
            outfile.writeln("\tpush " + std::to_string(op.push_val));

        else if (op.type == OP_PLUS)
        {
            outfile.writeln("\t; plus");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tadd rax, rbx");
            outfile.writeln("\tpush rax");
        }

        else if (op.type == OP_MINUS)
        {
            outfile.writeln("\t; minus");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tsub rbx, rax");
            outfile.writeln("\tpush rbx");
        }

        else if (op.type == OP_PRINT)
        {
            outfile.writeln("\t; print");
            outfile.writeln("\tpop rdi");
            outfile.writeln("\tcall print");
        }
    }
    
    // exit syscall at end of file
    outfile.writeln("\tmov rax, 60");
    outfile.writeln("\tmov rdi, 0");
    outfile.writeln("\tsyscall");
}
