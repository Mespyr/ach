#include "../include/lexer.h"

long unsigned int find_next_token_start_col(long unsigned int column_number, std::string line)
{
    while (column_number < line.length() && std::isspace(line.at(column_number)) && line.at(column_number) != '#')
        column_number++;

    return column_number;
}

long unsigned int find_token_end_col(long unsigned int column_number, std::string line)
{
    while (column_number < line.length() && !std::isspace(line.at(column_number)) && line.at(column_number) != '#')
        column_number++;

    return column_number;
}

long unsigned int find_string_end_col(long unsigned int column_number, std::string line)
{
    // start column_number after first quote
    column_number++;

    while (column_number < line.length() && line.at(column_number) != '"')
    {
        // check if escape char was found and skip next char from parsing if not at EOL
        if (line.at(column_number) == '\\')
            column_number++;

        column_number++;
    }
    
    // return position where '"' was found
    return column_number;
}

std::vector<Token> tokenize_line(std::string line, std::string file_location, long unsigned int line_number)
{
    std::vector<Token> tokens;

    // get starting position of first token
    long unsigned int column_number_start = find_next_token_start_col(0, line);
    long unsigned int column_number_end;

    while (column_number_start < line.length())
    {
        // if there is a comment, then ignore rest of line
        if (line.at(column_number_start) == '#') 
            return tokens;

        // if token is a string
        if (line.at(column_number_start) == '"')
        {
            // get end position of string
            column_number_end = find_string_end_col(column_number_start, line);

            // check if col_end is at end of line
            if (column_number_end == line.length())
            {
                print_lexing_error(
                    Location(line_number, column_number_start, column_number_end, line, file_location),
                    "unexpected EOL while tokenizing string"
                );
                exit(1);
            }
            
            // inc col_end so it points to after '"'
            column_number_end++;
        }
        else column_number_end = find_token_end_col(column_number_start, line); 

        tokens.push_back(Token(
            line.substr(column_number_start, column_number_end - column_number_start),
            Location(line_number, column_number_start, column_number_end, line, file_location)
        ));

        // get start position of next token
        column_number_start = find_next_token_start_col(column_number_end + 1, line);
    }

    return tokens;
}

std::vector<Token> tokenize_file(std::string file_location)
{
    // open file
    std::fstream file;
    file.open(file_location, std::ios::in);

    // exit if file doesn't exist
    if (!file)
    {
        print_error_with_no_location("couldn't open file '" + file_location + "'");
        exit(1);
    }

    std::vector<Token> tokens;
    long unsigned int line_number = 0;
    std::string line;

    while (file)
    {
        // read next line from file
        std::getline(file, line);
        line_number++;

        // get tokens on line
        std::vector<Token> line_toks = tokenize_line(line, file_location, line_number);

        // add all tokens from line to main token vector
        tokens.insert(tokens.end(), line_toks.begin(), line_toks.end());

        // clear line_toks
        line_toks.clear();
    }

    return tokens;
}
