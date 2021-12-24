#ifndef ILU_PARSER_H
#define ILU_PARSER_H

#include "ops.h"
#include "util.h"
#include "token.h"
#include "error.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>

std::map<std::string, std::vector<Op>> parse_tokens(std::vector<Token> tokens);
std::vector<Op> convert_tokens_to_ops(std::vector<Token> tokens, std::map<std::string, std::vector<Token>> basic_program);
std::vector<Op> link_ops(std::vector<Op> ops);

#endif
