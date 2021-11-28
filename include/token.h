#ifndef ILU_TOKEN_H
#define ILU_TOKEN_H

#include <string>

class Token
{
public:
    Token(std::string value, std::string filename, int line, int col);

    std::string value;
    std::string file_location;
    int line_num, col_num;
};

#endif
