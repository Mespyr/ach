#include "../include/parser.h"

std::vector<Op> parse_tokens(std::vector<Token> tokens)
{
    std::vector<Op> program;

    for (Token tok : tokens)
    {

        // arithmetic
        if (tok.value == "+") 
            program.push_back(plus());
        else if (tok.value == "-") 
            program.push_back(minus());
        else if (tok.value == "*")
            program.push_back(mul());

        // print
        else if (tok.value == ".")
            program.push_back(print());

        // push number
        else if (is_number(tok.value)) 
            program.push_back(push(atoi(tok.value.c_str())));

        else
        {
            std::cerr << ERROR_COLOR << "[" << tok.location << "] line " << tok.line_number << ", column " << tok.col_number << ": Unknown keyword '" << tok.value << "'" << RESET_COLOR << std::endl;
            exit(1);
        }
    }

    return program;
}
