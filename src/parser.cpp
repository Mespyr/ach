#include "../include/parser.h"

std::vector<Op> link_ops(std::vector<Op> ops)
{
    std::vector<int> ip_stack;
    for (int ip = 0; ip < ops.size(); ip++)
    {
        Op current_op = ops.at(ip);

        if (current_op.type == OP_IF)
            ip_stack.push_back(ip);

        else if (current_op.type == OP_WHILE)
            ip_stack.push_back(ip);

        else if (current_op.type == OP_DO)
        {
            if (ip_stack.size() == 0)
            {
                print_op_error(current_op, "Unexpected 'do' keyword");
                exit(1);
            }
            int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            if (ops.at(linker_ip).type != OP_WHILE)
            {
                print_op_error(current_op, "Unexpected 'do' keyword");
                exit(1);
            }

            current_op.reference_ip = linker_ip;

            ops.at(ip) = current_op;
            ip_stack.push_back(ip);
        }

        else if (current_op.type == OP_ELSE)
        {
            if (ip_stack.size() == 0)
            {
                print_op_error(current_op, "Unexpected 'else' keyword");
                exit(1);
            }

            int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            Op linker_op = ops.at(linker_ip);
            if (linker_op.type != OP_IF)
            {
                print_op_error(current_op, "Unexpected 'else' keyword");
                exit(1);
            }

            linker_op.reference_ip = ip;

            ops.at(linker_ip) = linker_op;
            ip_stack.push_back(ip);
        }

        else if (current_op.type == OP_END)
        {
            if (ip_stack.size() == 0)
            {
                print_op_error(current_op, "Unexpected 'end' keyword");
                exit(1);
            }

            int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            Op linker_op = ops.at(linker_ip);
            if (linker_op.type == OP_IF || linker_op.type == OP_ELSE)
            {
                current_op.reference_ip = -1;
                ops.at(ip) = current_op;

                linker_op.reference_ip = ip;
                ops.at(linker_ip) = linker_op;

            }

            else if (linker_op.type == OP_DO)
            {
                current_op.reference_ip = linker_op.reference_ip;
                ops.at(ip) = current_op;

                linker_op.reference_ip = ip;
                ops.at(linker_ip) = linker_op;
            }

            else
            {
                print_op_error(current_op, "Unexpected 'end' keyword");
                exit(1);
            }
        }
    }

    return ops;
}

std::vector<Op> parse_tokens(std::vector<Token> tokens)
{
    std::vector<Op> program;

    for (Token tok : tokens)
    {
        // ops
        if (tok.value == "+") 
            program.push_back(Op(OP_PLUS, tok));

        else if (tok.value == "-") 
            program.push_back(Op(OP_MINUS, tok));

        else if (tok.value == "*")
            program.push_back(Op(OP_MUL, tok));

        else if (tok.value == "/")
            program.push_back(Op(OP_DIV, tok));

        else if (tok.value == "=")
            program.push_back(Op(OP_EQUAL, tok));

        else if (tok.value == ">")
            program.push_back(Op(OP_GREATER, tok));

        else if (tok.value == "<")
            program.push_back(Op(OP_LESS, tok));

        else if (tok.value == ">=")
            program.push_back(Op(OP_GREATER_EQ, tok));

        else if (tok.value == "<=")
            program.push_back(Op(OP_LESS_EQ, tok));

        else if (tok.value == "dump")
            program.push_back(Op(OP_DUMP, tok));

        else if (tok.value == "pop")
            program.push_back(Op(OP_POP, tok));

        else if (tok.value == "dup")
            program.push_back(Op(OP_DUP, tok));

        else if (tok.value == "swp")
            program.push_back(Op(OP_SWP, tok));

        else if (tok.value == "rot")
            program.push_back(Op(OP_ROT, tok));

        else if (tok.value == "over")
            program.push_back(Op(OP_OVER, tok));

        else if (tok.value == "mem")
            program.push_back(Op(OP_MEM, tok));

        else if (tok.value == "read")
            program.push_back(Op(OP_READ, tok));
        
        else if (tok.value == "write")
            program.push_back(Op(OP_WRITE, tok));

        else if (tok.value == "<<")
            program.push_back(Op(OP_SHIFT_LEFT, tok));

        else if (tok.value == ">>")
            program.push_back(Op(OP_SHIFT_RIGHT, tok));

        else if (tok.value == "orb")
            program.push_back(Op(OP_ORB, tok));

        else if (tok.value == "andb")
            program.push_back(Op(OP_ANDB, tok));

        else if (tok.value == "syscall1")
            program.push_back(Op(OP_SYSCALL1, tok));

        else if (tok.value == "syscall2")
            program.push_back(Op(OP_SYSCALL2, tok));

        else if (tok.value == "syscall3")
            program.push_back(Op(OP_SYSCALL3, tok));

        else if (tok.value == "syscall4")
            program.push_back(Op(OP_SYSCALL4, tok));

        else if (tok.value == "syscall5")
            program.push_back(Op(OP_SYSCALL5, tok));

        else if (tok.value == "syscall6")
            program.push_back(Op(OP_SYSCALL6, tok));

        // keywords
        else if (tok.value == "while")
            program.push_back(Op(OP_WHILE, tok));

        else if (tok.value == "do")
            program.push_back(Op(OP_DO, tok));

        else if (tok.value == "if")
            program.push_back(Op(OP_IF, tok));

        else if (tok.value == "end")
            program.push_back(Op(OP_END, tok));

        else if (tok.value == "else")
            program.push_back(Op(OP_ELSE, tok));

        // push
        else if (is_number(tok.value)) 
            program.push_back(Op(OP_PUSH, atoi(tok.value.c_str()), tok));

        else
        {
            print_token_error(tok, "Unknown keyword '" + tok.value + "'");
            exit(1);
        }
    }

    return link_ops(program);
}
