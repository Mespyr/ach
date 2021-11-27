#ifndef ILU_PARSER_H
#define ILU_PARSER_H

#include "ops.h"
#include "util.h"
#include <vector>
#include <string>
#include <iostream>

std::vector<Op> parse_tokens(std::vector<std::string> tokens);

#endif
