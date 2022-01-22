#ifndef ILU_PARSER_H
#define ILU_PARSER_H

#include "ops.h"
#include "util.h"
#include "token.h"
#include "error.h"
#include "function.h"
#include "lexer.h"
#include "file.h"
#include "program.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

void print_error_if_illegal_word(Token tok, std::map<std::string, Const> consts, std::map<std::string, Function> functions);
std::vector<Op> link_ops(std::vector<Op> ops);
Op convert_token_to_op(Token tok, std::map<std::string, Function> functions, std::map<std::string, Const> consts);
Program parse_tokens(std::vector<Token> tokens);

#endif
