#include "../include/parser.h"

std::vector<Op> parse_tokens(std::vector<Token> tokens)
{
    std::vector<Op> program;

    for (Token tok : tokens)
    {
        if (tok.value == "+") 
            program.push_back(plus());

        else if (tok.value == "-") 
            program.push_back(minus());

        else if (tok.value == ".")
            program.push_back(print());

        else if (is_number(tok.value)) 
            program.push_back(push(atoi(tok.value.c_str())));

        else
        {
            std::cerr << "[" << tok.file_location << "] ERROR: line " << tok.line_num << ", column " << tok.col_num << ": Unknown keyword '" << tok.value << "'" << std::endl;
            exit(1);
        }
    }

    return program;
}
