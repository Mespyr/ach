#ifndef ILU_ERROR_H
#define ILU_ERROR_H

#include "token.h"
#include "ops.h"
#include "util.h"
#include <iostream>
#include <string>

void print_token_error(Token token, std::string message);
void print_op_error(Op op, std::string message);
void print_lexing_error(std::string file_location, int line_number, int column_number_start, int column_number_end, std::string line, std::string message);

#endif
