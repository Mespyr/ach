#include "../include/error.h"

void print_token_error(Token token, std::string message)
{
    std::cout << ERROR_COLOR << "[" << token.file_location << "] error on line " << token.line_number << " column " << token.column_number_start << ": " << message << std::endl;
    std::cout << "  " << token.line << std::endl;

    for (int i = 0; i <= token.column_number_start+1; i++)
        std::cout << " ";

    for (int i = 0; i < token.value.length(); i++)
        std::cout << "^";

    std::cout << "\n";
}
