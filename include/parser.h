#ifndef ILU_PARSER_H
#define ILU_PARSER_H

#include "ops.h"
#include <vector>
#include <string>

std::vector<Op> parse_tokens(std::vector<std::string> tokens);

#endif
