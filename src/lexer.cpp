#include "../include/lexer.h"

int find_next_token_start_col(int column_number, std::string line)
{
    while (
        column_number < line.length() && 
        std::isspace(line.at(column_number)) && 
        line.at(column_number) != '#'
    ) column_number++;

    return column_number;
}

int find_token_end_col(int column_number, std::string line)
{
    while (
        column_number < line.length() && 
        !std::isspace(line.at(column_number)) && 
        line.at(column_number) != '#'
    ) column_number++;

    return column_number;
}

int find_string_end_col(int column_number, std::string line)
{
    // start column_number after first quote
    column_number++;

    while (column_number < line.length() && line.at(column_number) != '"')
    {
        // check if escape char was found and skip next char from parsing if not at EOL
        if (line.at(column_number) == '\\')
            if (column_number + 1 < line.length()) 
                column_number++;

        column_number++;
    }
   
    // if we reached EOL before finding '"'
    if (column_number == line.length())
        return column_number;

    // return position where '"' was found
    return column_number++;
}

std::vector<Token> tokenize_line(std::string line, std::string filename, int line_number)
{
    std::vector<Token> tokens;

    // get starting position of first token
    int column_number = find_next_token_start_col(0, line);
    int col_end;

    while (column_number < line.length())
    {
        // if there is a comment, then ignore rest of line
        if (line.at(column_number) == '#') 
            return tokens;

        // if token is a string
        if (line.at(column_number) == '"')
        {
            // get end position of string
            col_end = find_string_end_col(column_number, line);

            // check if col_end is at end of line
            if (col_end == line.length())
            {
                print_lexing_error(filename, line_number, column_number, col_end, line, "unexpected EOL while tokenizing string");
                exit(1);
            }
        }
        else col_end = find_token_end_col(column_number, line); 

        tokens.push_back(Token(
            line.substr(column_number, col_end - column_number),  // token value
            filename,                                             // file location
            line,                                                 // line value
            line_number,                                          // line number
            column_number,                                        // column number start value
            col_end                                               // column number end value
        ));

        // get start position of next token
        column_number = find_next_token_start_col(col_end, line);
    }

    return tokens;
}

std::vector<Token> tokenize_file(std::string filename)
{
    // open file
    std::fstream file;
    file.open(filename, std::ios::in);

    // exit if file doesn't exist
    if (!file)
    {
        std::cout << ERROR_COLOR << "[error] couldn't open file '" << filename << "'" << RESET_COLOR << std::endl;
        exit(1);
    }

    std::vector<Token> tokens;
    int line_number = 0;
    std::string line;

    while (file)
    {
        // read next line from file
        std::getline(file, line);
        line_number++;

        // get tokens on line
        std::vector<Token> line_toks = tokenize_line(line, filename, line_number);

        // add all tokens from line to main token vector
        tokens.insert(tokens.end(),
            line_toks.begin(),
            line_toks.end());
        // clear line_toks
        line_toks.clear();
    }

    return tokens;
}
