#ifndef ILU_ERROR_H
#define ILU_ERROR_H

#include "location.h"
#include "token.h"
#include "ops.h"
#include "util.h"
#include "datatype.h"
#include <iostream>
#include <string>
#include <vector>

void print_error_with_no_location(std::string message);
void print_lexing_error(Location loc, std::string message);
void print_token_error(Token token, std::string message);

void print_op_error(Op op, std::string message);
void print_op_note(Op op, std::string message);
void print_not_enough_arguments_error(Op op, int expected, int actual, std::string name, std::string fullname = "", bool is_block = false);

void print_invalid_combination_of_types_error(Op op, std::vector<DATATYPE> types, std::string name, std::string fullname = "", bool is_block = false, bool is_func = false);
void print_invalid_type_error(Op op, DATATYPE expected, DATATYPE actual, std::string name, std::string fullname = "", bool is_block = false, bool is_func = false);

#endif
