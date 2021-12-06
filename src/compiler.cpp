#include "../include/compiler.h"
#include <cstdlib>

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


    int ip = 0;
    for (Op op : program) 
    {
        // builtin functions/operations
        if (op.type == OP_PUSH)
        {
            outfile.writeln("\t; push");
            outfile.writeln("\tpush " + std::to_string(op.push_val));
        }

        else if (op.type == OP_POP)
        {
            outfile.writeln("\t; pop");
            outfile.writeln("\tpop rax");
        }

        else if (op.type == OP_PRINT)
        {
            outfile.writeln("\t; print");
            outfile.writeln("\tpop rdi");
            outfile.writeln("\tcall print");
        }

        else if (op.type == OP_DUP)
        {
            outfile.writeln("\t; dup");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpush rax");
            outfile.writeln("\tpush rax");
        }

        else if (op.type == OP_IF)
        {
            outfile.writeln("\t; if");
            outfile.writeln("\tpop rax");
            outfile.writeln("\ttest rax, rax");
            outfile.writeln("\tjz addr_" + std::to_string(op.reference_ip));
        }

        else if (op.type == OP_ELSE)
        {
            outfile.writeln("\t; else");
            outfile.writeln("\tjmp addr_" + std::to_string(op.reference_ip));
            outfile.writeln("addr_" + std::to_string(ip) + ":");
        }

        else if (op.type == OP_END)
            outfile.writeln("addr_" + std::to_string(ip) + ":");

        // arithmetics
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

        else if (op.type == OP_MUL)
        {
            outfile.writeln("\t; multiply");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tmul rbx");
            outfile.writeln("\tpush rax");
        }

        else if (op.type == OP_DIV)
        {
            outfile.writeln("\t; division");
            outfile.writeln("\txor rdx, rdx");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tdiv rbx");
            outfile.writeln("\tpush rax");
            outfile.writeln("\tpush rdx");
        }

        // comparisons
        else if (op.type == OP_EQUAL)
        {
            outfile.writeln("\t; equal");
            outfile.writeln("\tmov rcx, 0");
            outfile.writeln("\tmov rdx, 1");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tcmp rax, rbx");
            outfile.writeln("\tcmove rcx, rdx");
            outfile.writeln("\tpush rcx");
        }

        else if (op.type == OP_GREATER)
        {
            outfile.writeln("\t; greater");
            outfile.writeln("\tmov rcx, 0");
            outfile.writeln("\tmov rdx, 1");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tcmp rax, rbx");
            outfile.writeln("\tcmovg rcx, rdx");
            outfile.writeln("\tpush rcx");
        }

        else if (op.type == OP_LESS)
        {
            outfile.writeln("\t; less");
            outfile.writeln("\tmov rcx, 0");
            outfile.writeln("\tmov rdx, 1");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tcmp rax, rbx");
            outfile.writeln("\tcmovl rcx, rdx");
            outfile.writeln("\tpush rcx");
        }

        ip++;
    }
    
    // exit syscall at end of file
    outfile.writeln("\t; exit");
    outfile.writeln("\tmov rax, 60");
    outfile.writeln("\tmov rdi, 0");
    outfile.writeln("\tsyscall");
}
