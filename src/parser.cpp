#include "../include/parser.h"

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

        // print
        else if (tok.value == ".")
            program.push_back(Op(OP_PRINT));

        // pop number
        else if (tok.value == ",")
            program.push_back(Op(OP_POP));

        // push number
        else if (is_number(tok.value)) 
            program.push_back(Op(OP_PUSH, atoi(tok.value.c_str())));

        else
        {
            std::cerr << ERROR_COLOR << "[" << tok.location << "] line " << tok.line_number << ", column " << tok.col_number << ": Unknown keyword '" << tok.value << "'" << RESET_COLOR << std::endl;
            exit(1);
        }
    }

    return program;
}
