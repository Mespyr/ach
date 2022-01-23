#include "../include/compiler.h"

void compile_to_asm(Program program, std::string output_filename, ASSEMBLER assembler)
{
    static_assert(OP_COUNT == 52, "unhandled op types in compile_to_asm()");
    static_assert(ASSEMBLER_COUNT == 2, "unhandled assemblers in compile_to_asm()");

    File outfile(output_filename, MODE_WRITE);


    // write boilerplate into file
    if (assembler == FASM)
    {
        outfile.writeln("format ELF64 executable 3");
        outfile.writeln("segment readable executable");
        outfile.writeln("entry start");
    }
    else
    {
        outfile.writeln("BITS 64");
        outfile.writeln("section .text");
        outfile.writeln("global _start");
    }
    outfile.writeln("dump:");
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

    std::vector<std::string> strings;
    
    for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++)
    {
        Function function = fn_key->second;

        outfile.writeln("; function " + fn_key->first);
        outfile.writeln("func_addr_" + std::to_string(function.addr) + ":");
        outfile.writeln("\tmov [ret_stack_rsp], rsp");
        outfile.writeln("\tmov rsp, rax");

        for (long unsigned int ip = 0; ip < function.ops.size(); ip++)
        {
            Op op = function.ops.at(ip);
            
            // debugging
            if (op.type == OP_DUMP)
            {
                outfile.writeln("\t; OP_DUMP");
                outfile.writeln("\tpop rdi");
                outfile.writeln("\tcall dump");
            }
            
            // arithmetics
            else if (op.type == OP_PLUS)
            {
                outfile.writeln("\t; OP_PLUS");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tadd rbx, rax");
                outfile.writeln("\tpush rbx");
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
            
            // comparisons
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
            else if (op.type == OP_GREATER_EQ)
            {
                outfile.writeln("\t; OP_GREATER_EQ");
                outfile.writeln("\tmov rcx, 0");
                outfile.writeln("\tmov rdx, 1");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tcmp rax, rbx");
                outfile.writeln("\tcmovge rcx, rdx");
                outfile.writeln("\tpush rcx");
            }
            else if (op.type == OP_LESS_EQ)
            {
                outfile.writeln("\t; OP_LESS_EQ");
                outfile.writeln("\tmov rcx, 0");
                outfile.writeln("\tmov rdx, 1");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tcmp rax, rbx");
                outfile.writeln("\tcmovle rcx, rdx");
                outfile.writeln("\tpush rcx");
            }
            else if (op.type == OP_NOT_EQ)
            {
                outfile.writeln("\t; OP_NOT_EQ");
                outfile.writeln("\tmov rcx, 0");
                outfile.writeln("\tmov rdx, 1");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tcmp rax, rbx");
                outfile.writeln("\tcmovne rcx, rdx");
                outfile.writeln("\tpush rcx");
            }
            else if (op.type == OP_NOT)
            {
                outfile.writeln("\t; OP_NOT");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tnot rax");
                outfile.writeln("\tpush rax");
            }
            else if (op.type == OP_AND)
            {
                outfile.writeln("\t; OP_AND");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tand rbx, rax");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_OR)
            {
                outfile.writeln("\t; OP_OR");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tor rbx, rax");
                outfile.writeln("\tpush rbx");
            }
            
            // stack manipulation
            else if (op.type == OP_POP)
            {
                outfile.writeln("\t; OP_POP");
                outfile.writeln("\tpop rax");
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
            
            // memory
            else if (op.type == OP_READ8)
            {
                outfile.writeln("\t; OP_READ8");
                outfile.writeln("\tpop rax");
                outfile.writeln("\txor rbx, rbx");
                outfile.writeln("\tmov bl, [rax]");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_WRITE8)
            {
                outfile.writeln("\t; OP_WRITE8");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tmov [rax], bl");
            }
            else if (op.type == OP_READ16)
            {
                outfile.writeln("\t; OP_READ16");
                outfile.writeln("\tpop rax");
                outfile.writeln("\txor rbx, rbx");
                outfile.writeln("\tmov bx, [rax]");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_WRITE16)
            {
                outfile.writeln("\t; OP_WRITE16");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tmov [rax], bx");
            }
            else if (op.type == OP_READ32)
            {
                outfile.writeln("\t; OP_READ32");
                outfile.writeln("\tpop rax");
                outfile.writeln("\txor rbx, rbx");
                outfile.writeln("\tmov ebx, [rax]");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_WRITE32)
            {
                outfile.writeln("\t; OP_WRITE32");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tmov [rax], ebx");
            }
            else if (op.type == OP_READ64)
            {
                outfile.writeln("\t; OP_READ64");
                outfile.writeln("\tpop rax");
                outfile.writeln("\txor rbx, rbx");
                outfile.writeln("\tmov rbx, [rax]");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_WRITE64)
            {
                outfile.writeln("\t; OP_WRITE64");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tmov [rax], rbx");
            }

            // argv
            else if (op.type == OP_ARGV)
            {
                outfile.writeln("\t; OP_ARGV");
                outfile.writeln("\tmov rax, [args_ptr]");
                outfile.writeln("\tadd rax, 8");
                outfile.writeln("\tpush rax");
            }
            else if (op.type == OP_ARGC)
            {
                outfile.writeln("\t; OP_ARGC");
                outfile.writeln("\tmov rax, [args_ptr]");
                outfile.writeln("\tmov rax, [rax]");
                outfile.writeln("\tpush rax");
            }

            // bitwise
            else if (op.type == OP_SHIFT_LEFT)
            {
                outfile.writeln("\t; OP_SHIFT_LEFT");
                outfile.writeln("\tpop rcx");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tshl rbx, cl");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_SHIFT_RIGHT)
            {
                outfile.writeln("\t; OP_SHIFT_RIGHT");
                outfile.writeln("\tpop rcx");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tshr rbx, cl");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_ORB)
            {
                outfile.writeln("\t; OP_ORB");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tor rbx, rax");
                outfile.writeln("\tpush rbx");
            }
            else if (op.type == OP_ANDB)
            {
                outfile.writeln("\t; OP_ANDB");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rbx");
                outfile.writeln("\tand rbx, rax");
                outfile.writeln("\tpush rbx");
            }
            
            // syscalls
            else if (op.type == OP_SYSCALL1)
            {
                outfile.writeln("\t; OP_SYSCALL1");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rdi");
                outfile.writeln("\tsyscall");
            }
            else if (op.type == OP_SYSCALL2)
            {
                outfile.writeln("\t; OP_SYSCALL2");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rsi");
                outfile.writeln("\tpop rdi");
                outfile.writeln("\tsyscall");
            }
            else if (op.type == OP_SYSCALL3)
            {
                outfile.writeln("\t; OP_SYSCALL3");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop rdx");
                outfile.writeln("\tpop rsi");
                outfile.writeln("\tpop rdi");
                outfile.writeln("\tsyscall");
            }
            else if (op.type == OP_SYSCALL4)
            {
                outfile.writeln("\t; OP_SYSCALL4");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop r10");
                outfile.writeln("\tpop rdx");
                outfile.writeln("\tpop rsi");
                outfile.writeln("\tpop rdi");
                outfile.writeln("\tsyscall");
            }
            else if (op.type == OP_SYSCALL5)
            {
                outfile.writeln("\t; OP_SYSCALL5");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop r8");
                outfile.writeln("\tpop r10");
                outfile.writeln("\tpop rdx");
                outfile.writeln("\tpop rsi");
                outfile.writeln("\tpop rdi");
                outfile.writeln("\tsyscall");
            }
            else if (op.type == OP_SYSCALL6)
            {
                outfile.writeln("\t; OP_SYSCALL6");
                outfile.writeln("\tpop rax");
                outfile.writeln("\tpop r9");
                outfile.writeln("\tpop r8");
                outfile.writeln("\tpop r10");
                outfile.writeln("\tpop rdx");
                outfile.writeln("\tpop rsi");
                outfile.writeln("\tpop rdi");
                outfile.writeln("\tsyscall");
            }

            // keywords
            else if (op.type == OP_WHILE)
            {
                outfile.writeln("\t; OP_WHILE");
                outfile.writeln("addr_" + std::to_string(function.addr) + "_" + std::to_string(ip) + ":");
            }
            else if (op.type == OP_DO)
            {
                outfile.writeln("\t; OP_DO");
                outfile.writeln("\tpop rax");
                outfile.writeln("\ttest rax, rax");
                outfile.writeln("\tjz addr_" + std::to_string(function.addr) + "_" + std::to_string(op.int_operand));
            }
            else if (op.type == OP_IF)
            {
                outfile.writeln("\t; OP_IF");
                outfile.writeln("\tpop rax");
                outfile.writeln("\ttest rax, rax");
                outfile.writeln("\tjz addr_" + std::to_string(function.addr) + "_" + std::to_string(op.int_operand));
            }
            else if (op.type == OP_ELSE)
            {
                outfile.writeln("\t; OP_ELSE");
                outfile.writeln("\tjmp addr_" + std::to_string(function.addr) + "_" + std::to_string(op.int_operand));
                outfile.writeln("addr_" + std::to_string(function.addr) + "_" + std::to_string(ip) + ":");
            }
            else if (op.type == OP_END)
            {
                outfile.writeln("\t; OP_END");
                if (op.link_back)
                    outfile.writeln("\tjmp addr_" + std::to_string(function.addr) + "_" + std::to_string(op.int_operand));
                outfile.writeln("addr_" + std::to_string(function.addr) + "_" + std::to_string(ip) + ":");
            }

            // lang subset keywords
            else if (op.type == OP_OFFSET || op.type == OP_RESET)
            {
                print_error("OP_OFFSET and OP_RESET are unreachable because they should be handled in the parsing step. This is probably a bug.");
                exit(1);
            }

            // other 
            else if (op.type == OP_PUSH_INT)
            {
                outfile.writeln("\t; OP_PUSH_INT");
                outfile.writeln("\tmov rax, " + std::to_string(op.int_operand));
                outfile.writeln("\tpush rax");
            }
            else if (op.type == OP_PUSH_STR)
            {
                strings.push_back(op.str_operand);
                outfile.writeln("\t; OP_PUSH_STR");
                outfile.writeln("\tmov rax, " + std::to_string(op.str_operand.length()));
                outfile.writeln("\tpush rax");
                outfile.writeln("\tpush str_" + std::to_string(strings.size()-1));
            }
            else if (op.type == OP_FUNCTION_CALL)
            {
                assert(program.functions.count(op.str_operand));

                outfile.writeln("\t; OP_FUNCTION_CALL");
                outfile.writeln("\tmov rax, rsp");
                outfile.writeln("\tmov rsp, [ret_stack_rsp]");
                outfile.writeln("\tcall func_addr_" + std::to_string(program.functions.at(op.str_operand).addr));
                outfile.writeln("\tmov [ret_stack_rsp], rsp");
                outfile.writeln("\tmov rsp, rax");
            }
            else if (op.type == OP_PUSH_GLOBAL_MEM)
            {
                outfile.writeln("\t; OP_PUSH_GLOBAL_MEM");
                outfile.writeln("\tmov rax, mem");
                outfile.writeln("\tadd rax, " + std::to_string(op.int_operand));
                outfile.writeln("\tpush rax");
            }
        }
        outfile.writeln("\t; return");
        outfile.writeln("\tmov rax, rsp");
        outfile.writeln("\tmov rsp, [ret_stack_rsp]");
        outfile.writeln("\tret");
    }

    if (assembler == FASM)
        outfile.writeln("start:");
    else
        outfile.writeln("_start:");
    outfile.writeln("\tmov [args_ptr], rsp");
    outfile.writeln("\tmov rax, ret_stack_end");
    outfile.writeln("\tmov [ret_stack_rsp], rax");
    outfile.writeln("\tcall func_addr_" + std::to_string(program.functions.at("main").addr));
    outfile.writeln("\tmov rax, 60");
    outfile.writeln("\tmov rdi, 0");
    outfile.writeln("\tsyscall");

    // data section
    if (assembler == FASM)
        outfile.writeln("segment readable writable");
    else
        outfile.writeln("section .data");

    for (long unsigned int i = 0; i < strings.size(); i++)
    {
        std::stringstream ss;

    for (char c : strings.at(i))
            ss << (int) c << ",";
        ss << "0";
        outfile.writeln("str_" + std::to_string(i) + ": db " + ss.str());
    }

    // bss section
    if (assembler == FASM)
    {
        outfile.writeln("args_ptr: rq 1");
        outfile.writeln("ret_stack_rsp: rq 1");
        outfile.writeln("ret_stack: rb 4096");
        outfile.writeln("ret_stack_end:");
        outfile.writeln("mem: rb " + std::to_string(program.memory_capacity));
    }
    else
    {
        outfile.writeln("section .bss");
        outfile.writeln("args_ptr: resq 1");
        outfile.writeln("ret_stack_rsp: resq 1");
        outfile.writeln("ret_stack: resb 4096");
        outfile.writeln("ret_stack_end:");
        outfile.writeln("mem: resb " + std::to_string(program.memory_capacity));
    }
}
