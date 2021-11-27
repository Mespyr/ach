#include "../include/lexer.h"

std::vector<std::string> tokenize(std::string code)
{
    std::vector<std::string> tokens;
    std::string current_word;

    for (char current_char : code)
    {
        if (current_char == ' ' || current_char == '\n' || current_char == '\t')
            if (!current_word.empty())
            {
                tokens.push_back(current_word);
                current_word.clear();
                continue;
            }
        current_word.push_back(current_char);
    }

    return tokens;
}
