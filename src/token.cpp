#include "../include/token.h"

Token::Token(std::string value, std::string filename, int line, int col)
{
    this->value = value;
    this->file_location = filename;
    this->line_num = line;
    this->col_num = col;
}
