#ifndef ILU_LEXER_H
#define ILU_LEXER_H

#include "token.h"
#include <fstream>
#include <vector>
#include <string>

int get_next_token_start(int column_number, std::string line);
int get_token_end(int column_number, std::string line);
std::vector<Token> get_tokens_from_line(std::string line, std::string filename, int line_number);
std::vector<Token> load_tokens_from_file(std::string filename);

#endif
