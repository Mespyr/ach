#ifndef _ACH_PARSER_H_
#define _ACH_PARSER_H_

#include "ops.h"
#include "token.h"
#include "error.h"
#include "lexer.h"
#include "file.h"
#include "program.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

struct ConstValueWithContext
{
	long long value;
	long long iota;
	long unsigned int i;
};

std::string add_escapes_to_string(std::string str);
std::vector<Op> link_ops(std::vector<Op> ops);
ConstValueWithContext eval_const_value(Program program, std::vector<Token> tokens, long unsigned int i, long long iota, Location definition_loc);
Op convert_token_to_op(Token tok, Program program, bool in_function = false, std::string current_func_name = "");
Program parse_tokens(std::vector<Token> tokens);

#endif
