#include "../include/error.h"

void print_token_error(Token token, std::string message)
{
    std::cout << ERROR_COLOR << "[" << token.loc.file_location << "] error on line " << token.loc.line_number << " column " << token.loc.column_number_start << ": " << message << std::endl;
    std::cout << token.loc.line << std::endl;

    for (int i = 0; i < token.loc.column_number_start; i++)
        std::cout << " ";

    for (int i = 0; i < token.loc.column_number_end - token.loc.column_number_start; i++)
        std::cout << "^";

    std::cout << RESET_COLOR << std::endl; 
}

void print_op_error(Op op, std::string message)
{
    std::cout << ERROR_COLOR << "[" << op.loc.file_location << "] error on line " << op.loc.line_number << " column " << op.loc.column_number_start << ": " << message << std::endl;
    std::cout << op.loc.line << std::endl;

    for (int i = 0; i < op.loc.column_number_start; i++)
        std::cout << " ";

    for (int i = 0; i < op.loc.column_number_end - op.loc.column_number_start; i++)
        std::cout << "^";

    std::cout << RESET_COLOR << std::endl;
}

void print_lexing_error(Location loc, std::string message)
{
    std::cout << ERROR_COLOR << "[" << loc.file_location << "] error on line " << loc.line_number << " column " << loc.column_number_start << ": " << message << std::endl;
    std::cout << loc.line << std::endl;

    for (int i = 0; i < loc.column_number_start; i++)
        std::cout << " ";

    for (int i = 0; i < loc.column_number_end - loc.column_number_start; i++)
        std::cout << "^";

    std::cout << RESET_COLOR << std::endl;
}
