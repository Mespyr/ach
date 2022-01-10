#ifndef ILU_ERROR_H
#define ILU_ERROR_H

#include "location.h"
#include "token.h"
#include "ops.h"
#include "util.h"
#include <iostream>
#include <string>

void print_token_error(Token token, std::string message);
void print_op_error(Op op, std::string message);
void print_lexing_error(Location loc, std::string message);
void print_error_with_no_location(std::string message);

#endif
