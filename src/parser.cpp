#include "../include/parser.h"

bool is_number(std::string s)
{
    if (s.at(0) == '-')
        s = s.substr(1);

    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

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
    }

    return program;
}
