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
        // ops
        if (op.type == OP_PUSH)
        {
            outfile.writeln("\t; OP_PUSH");
            outfile.writeln("\tpush " + std::to_string(op.push_val));
        }

        else if (op.type == OP_POP)
        {
            outfile.writeln("\t; OP_POP");
            outfile.writeln("\tpop rax");
        }

        else if (op.type == OP_PRINT)
        {
            outfile.writeln("\t; OP_PRINT");
            outfile.writeln("\tpop rdi");
            outfile.writeln("\tcall print");
        }

        else if (op.type == OP_PLUS)
        {
            outfile.writeln("\t; OP_PLUS");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tadd rax, rbx");
            outfile.writeln("\tpush rax");
        }

        else if (op.type == OP_MINUS)
        {
            outfile.writeln("\t; OP_MINUS");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tsub rbx, rax");
            outfile.writeln("\tpush rbx");
        }

        else if (op.type == OP_MUL)
        {
            outfile.writeln("\t; OP_MUL");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tmul rbx");
            outfile.writeln("\tpush rax");
        }

        else if (op.type == OP_DIV)
        {
            outfile.writeln("\t; OP_DIV");
            outfile.writeln("\txor rdx, rdx");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tdiv rbx");
            outfile.writeln("\tpush rax");
            outfile.writeln("\tpush rdx");
        }

        else if (op.type == OP_EQUAL)
        {
            outfile.writeln("\t; OP_EQUAL");
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
            outfile.writeln("\t; OP_GREATER");
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
            outfile.writeln("\t; OP_LESS");
            outfile.writeln("\tmov rcx, 0");
            outfile.writeln("\tmov rdx, 1");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tcmp rax, rbx");
            outfile.writeln("\tcmovl rcx, rdx");
            outfile.writeln("\tpush rcx");
        }

        else if (op.type == OP_DUP)
        {
            outfile.writeln("\t; OP_DUP");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpush rax");
            outfile.writeln("\tpush rax");
        }

        else if (op.type == OP_SWP)
        {
            outfile.writeln("\t; OP_SWP");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpush rax");
            outfile.writeln("\tpush rbx");
        }

        else if (op.type == OP_ROT)
        {
            outfile.writeln("\t; OP_ROT");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpop rcx");
            outfile.writeln("\tpush rbx");
            outfile.writeln("\tpush rax");
            outfile.writeln("\tpush rcx");
        }

        else if (op.type == OP_OVER)
        {
            outfile.writeln("\t; OP_OVER");
            outfile.writeln("\tpop rax");
            outfile.writeln("\tpop rbx");
            outfile.writeln("\tpush rbx");
            outfile.writeln("\tpush rax");
            outfile.writeln("\tpush rbx");

        }

        // keywords
        else if (op.type == OP_WHILE)
        {
            outfile.writeln("\t; OP_WHILE");
            outfile.writeln("addr_" + std::to_string(ip) + ":");
        }

        else if (op.type == OP_DO)
        {
            outfile.writeln("\t; OP_DO");
            outfile.writeln("\tpop rax");
            outfile.writeln("\ttest rax, rax");
            outfile.writeln("\tjz addr_" + std::to_string(op.reference_ip));
        }

        else if (op.type == OP_IF)
        {
            outfile.writeln("\t; OP_IF");
            outfile.writeln("\tpop rax");
            outfile.writeln("\ttest rax, rax");
            outfile.writeln("\tjz addr_" + std::to_string(op.reference_ip));
        }

        else if (op.type == OP_ELSE)
        {
            outfile.writeln("\t; OP_ELSE");
            outfile.writeln("\tjmp addr_" + std::to_string(op.reference_ip));
            outfile.writeln("addr_" + std::to_string(ip) + ":");
        }

        else if (op.type == OP_END)
        {
            outfile.writeln("\t; OP_END");
            if (op.reference_ip != -1)
                outfile.writeln("\tjmp addr_" + std::to_string(op.reference_ip));
            outfile.writeln("addr_" + std::to_string(ip) + ":");
        }


        ip++;
    }
    
    // exit syscall at end of file
    outfile.writeln("\t; exit");
    outfile.writeln("\tmov rax, 60");
    outfile.writeln("\tmov rdi, 0");
    outfile.writeln("\tsyscall");
}
