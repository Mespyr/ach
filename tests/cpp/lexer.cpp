#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Token
{
public:
    Token(std::string value, int line, int column, std::string line_value) :
        value(value), line(line), column(column), line_value(line_value) {}

    std::string value;
    int line;
    int column;
    std::string line_value;
};

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

std::vector<Token> lex_line(std::string line, int line_number)
{
    std::vector<Token> tokens;
    int column_number = get_next_token_start(0, line);
    int col_end;
    while (column_number < line.length())
    {
        col_end = get_token_end(column_number, line);
        tokens.push_back(Token(line.substr(column_number, col_end - column_number), line_number, column_number+1, line));
        column_number = get_next_token_start(col_end, line);
    }

    return tokens;
}

std::vector<Token> lex_file(std::string filename)
{
    std::fstream file;
    file.open(filename, std::ios::in);

    std::vector<Token> tokens;
    int line_number = 0;

    while (file)
    {
        std::string line;
        std::getline(file, line);
        line_number++;
        std::vector<Token> line_toks = lex_line(line, line_number);
        for (Token tok : line_toks)
            tokens.push_back(tok);
    }

    return tokens;
}

int main()
{
    std::vector<Token> tokens = lex_file("file.txt"); 
    for (Token tok : tokens)
        std::cout << tok.value << " (line: " << tok.line << ") (column: " << tok.column << ") " << tok.line_value << std::endl;
    return 0;
}
