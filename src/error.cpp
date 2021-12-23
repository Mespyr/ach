#include "../include/error.h"

void print_token_error(Token token, std::string message)
{
    std::cout << ERROR_COLOR << "[" << token.file_location << "] error on line " << token.line_number << " column " << token.column_number_start << ": " << message << std::endl;
    std::cout << token.line << std::endl;

    for (int i = 0; i < token.column_number_start; i++) std::cout << " ";
    for (int i = 0; i < token.column_number_end - token.column_number_start; i++) std::cout << "^";
    std::cout << RESET_COLOR << std::endl; 
}

void print_op_error(Op op, std::string message)
{
    std::cout << ERROR_COLOR << "[" << op.file_location << "] error on line " << op.line_number << " column " << op.column_number_start << ": " << message << std::endl;
    std::cout << op.line << std::endl;

    for (int i = 0; i < op.column_number_start; i++) std::cout << " ";
    for (int i = 0; i < op.column_number_end - op.column_number_start; i++) std::cout << "^";
    std::cout << RESET_COLOR << std::endl;
}

void print_lexing_error(std::string file_location, int line_number, int column_number_start, int column_number_end, std::string line, std::string message)
{
    std::cout << ERROR_COLOR << "[" << file_location << "] error on line " << line_number << " column " << column_number_start << ": " << message << std::endl;
    std::cout << line << std::endl;

    for (int i = 0; i < column_number_start; i++) std::cout << " ";
    for (int i = 0; i < column_number_end - column_number_start; i++) std::cout << "^";
    std::cout << RESET_COLOR << std::endl;
}
