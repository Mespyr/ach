#include "../include/ops.h"
#include "../include/util.h"
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
        std::cerr << ERROR_COLOR << "ERROR: no file provided for compilation." << RESET_COLOR << std::endl;
        exit(1);
    }

    std::vector<Token> tokens = load_tokens_from_file(argv[1]);
    std::vector<Op> program = parse_tokens(tokens);

    // compile
    std::cout << INFO_COLOR << "[info] generating asm from '" << argv[1] << "'." << RESET_COLOR << std::endl;
    compile_to_asm(program, "out.asm");

    // nasm
    std::cout << INFO_COLOR << "[info] generating object file." << RESET_COLOR << std::endl;
    exit_on_error(std::system("nasm -felf64 out.asm"));

    // link
    std::cout << INFO_COLOR << "[info] linking object file." << RESET_COLOR << std::endl;
    exit_on_error(std::system("ld -o out out.o"));
    
    // cleanup
    std::cout << INFO_COLOR << "[info] cleaning up object file" << RESET_COLOR << std::endl;
    exit_on_error(std::system("rm out.o"));

    return 0;
}
