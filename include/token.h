#ifndef ILU_TOKEN_H
#define ILU_TOKEN_H

#include <string>

class Token
{
public:
    Token(std::string value, std::string location, int line_number, int col_number) :
        value(value), location(location), line_number(line_number), col_number(col_number) {}

    std::string value;
    std::string location;
    int line_number, col_number;
};

#endif
