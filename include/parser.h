#ifndef ILU_PARSER_H
#define ILU_PARSER_H

#include "ops.h"
#include "util.h"
#include "token.h"
#include "error.h"
#include <vector>
#include <string>
#include <iostream>

std::vector<Op> parse_tokens(std::vector<Token> tokens);
std::vector<Op> link_ops(std::vector<Op> ops);

#endif
