#include "../include/parser.h"

std::vector<Op> link_ops(std::vector<Op> ops)
{
    std::vector<int> ip_stack;
    for (int ip = 0; ip < ops.size(); ip++)
    {
        Op current_op = ops.at(ip);

        if (current_op.type == OP_IF_START)
            ip_stack.push_back(ip);

        else if (current_op.type == OP_IF_END)
        {
            int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            Op linker_op = ops.at(linker_ip);
            if (linker_op.type != OP_IF_START)
                exit(1);

            linker_op.reference_ip = ip;

            ops.at(linker_ip) = linker_op;
        }
    }

    return ops;
}

std::vector<Op> parse_tokens(std::vector<Token> tokens)
{
    std::vector<Op> program;

    for (Token tok : tokens)
    {
        // arithmetic
        if (tok.value == "+") 
            program.push_back(Op(OP_PLUS));

        else if (tok.value == "-") 
            program.push_back(Op(OP_MINUS));

        else if (tok.value == "*")
            program.push_back(Op(OP_MUL));

        else if (tok.value == "/")
            program.push_back(Op(OP_DIV));

        // comparisons
        else if (tok.value == "=")
            program.push_back(Op(OP_EQUAL));

        // keywords
        else if (tok.value == ".")
            program.push_back(Op(OP_PRINT));

        else if (tok.value == ",")
            program.push_back(Op(OP_POP));

        else if (tok.value == "[")
            program.push_back(Op(OP_IF_START));

        else if (tok.value == "]")
            program.push_back(Op(OP_IF_END));

        // push
        else if (is_number(tok.value)) 
            program.push_back(Op(OP_PUSH, atoi(tok.value.c_str())));

        else
        {
            print_token_error(tok, "Unknown keyword '" + tok.value + "'");
            exit(1);
        }
    }

    return link_ops(program);
}
