#include "../include/lexer.h"

std::vector<Token> load_tokens_from_file(std::string filename)
{
    File file(filename, MODE_READ);
    std::string code = file.read();
    std::vector<Token> tokens;

    std::string current_word;
    int line_number = 1;
    int col_number = 0;
    int start_col_number;
    bool tokenizing_word = false;

    for (char current_char : code)
    {
        col_number++;
        if (std::isspace(current_char))
        {
            if (tokenizing_word)
            {
                tokenizing_word = false;
                tokens.push_back(Token(current_word, filename, line_number, start_col_number));
                current_word.clear();
            }
        }
        else
        {
            if (!tokenizing_word)
            {
                tokenizing_word = true;
                start_col_number = col_number;
            }
            current_word.push_back(current_char);
        }
        
        if (current_char == '\n')
        {
            line_number++;
            col_number = 0;
        }

    }

    return tokens;
}
