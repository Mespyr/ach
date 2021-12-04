#include "../include/lexer.h"

int get_next_token_start(int column_number, std::string line)
{
    while (column_number < line.length() && std::isspace(line.at(column_number)))
        column_number++;

    return column_number;
}

int get_token_end(int column_number, std::string line)
{
    while (column_number < line.length() && !std::isspace(line.at(column_number)))
        column_number++;

    return column_number;
}

std::vector<Token> get_tokens_from_line(std::string line, std::string filename, int line_number)
{
    std::vector<Token> tokens;
    int column_number = get_next_token_start(0, line);
    int col_end;

    while (column_number < line.length())
    {
        col_end = get_token_end(column_number, line);

        tokens.push_back(Token(
            line.substr(column_number, col_end - column_number),  // token
            filename,                                             // file location
            line,                                                 // line value
            line_number,                                          // line number
            column_number,                                        // column number start value
            col_end                                               // column number end value
        ));

        column_number = get_next_token_start(col_end, line);
    }

    return tokens;
}

std::vector<Token> load_tokens_from_file(std::string filename)
{
    std::fstream file;
    file.open(filename, std::ios::in);

    std::vector<Token> tokens;
    int line_number = 0;
    std::string line;

    while (file)
    {
        std::getline(file, line);
        line_number++;
        std::vector<Token> line_toks = get_tokens_from_line(line, filename, line_number);

        for (Token tok : line_toks)
            tokens.push_back(tok);
    }

    return tokens;
}
