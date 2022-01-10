#ifndef ILU_PARSER_H
#define ILU_PARSER_H

#include "ops.h"
#include "util.h"
#include "token.h"
#include "error.h"
#include "function.h"
#include "lexer.h"
#include "file.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

std::map<std::string, Function> parse_tokens(std::vector<Token> tokens);
std::vector<Op> convert_tokens_to_ops(std::vector<Token> tokens, std::map<std::string, std::vector<Token>> basic_program);
std::vector<Op> link_ops(std::vector<Op> ops);

#endif
