#include "../include/ops.h"
#include "../include/compiler.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include <cstdlib>
#include <vector>
#include <iostream>

void usage(char* exec_name)
{
    std::cout << "USAGE: " << exec_name << " <filename>" << std::endl;
}

int main(int argc, char* argv[]) 
{
    if (argc == 1)
    {
        usage(argv[0]);
        std::cerr << "ERROR: no file provided for compilation." << std::endl;
        exit(1);
    }

    std::vector<Token> tokens = load_tokens_from_file(argv[1]);
    std::vector<Op> program = parse_tokens(tokens);
    
    compile_to_asm(program, "out.asm");

    return 0;
}
