#ifndef ILU_LEXER_H
#define ILU_LEXER_H

#include "token.h"
#include "file.h"
#include <vector>
#include <string>

std::vector<Token> load_tokens_from_file(std::string filename);

#endif
