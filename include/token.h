#ifndef ILU_TOKEN_H
#define ILU_TOKEN_H

#include <string>

class Token
{
public: 
    Token(std::string value, std::string file_location, std::string line, int line_number, int column_number_start, int column_number_end) :
        value(value), 
        file_location(file_location), 
        line(line),
        line_number(line_number), 
        column_number_start(column_number_start),
        column_number_end(column_number_end)
    {}

    Token() {}

    std::string value, file_location, line;
    int line_number, column_number_start, column_number_end;
};

#endif
