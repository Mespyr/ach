#include "../include/parser.h"

std::vector<Op> parse_tokens(std::vector<std::string> tokens)
{
    std::vector<Op> program;

    for (std::string tok : tokens)
    {
        if (tok == "+") 
            program.push_back(plus());

        else if (tok == "-") 
            program.push_back(minus());

        else if (tok == ".")
            program.push_back(print());

        else if (is_number(tok)) 
            program.push_back(push(atoi(tok.c_str())));

        else
        {
            std::cerr << "[error] Unknown keyword '" << tok << "'" << std::endl;
            exit(1);
        }
    }

    return program;
}
