#include "../include/ops.h"
#include "../include/util.h"
#include "../include/compiler.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>

void usage(char* exec_name)
{
    std::cout << "USAGE: " << exec_name << " <filename>" << std::endl;
}

int main(int argc, char* argv[]) 
{
    if (argc == 1)
    {
        usage(argv[0]);
        std::cerr << ERROR_COLOR << "[error] no file provided for compilation." << RESET_COLOR << std::endl;
        exit(1);
    }

    std::vector<Token> tokens = tokenize_file(argv[1]);
    std::map<std::string, Function> program = parse_tokens(tokens);

    ASSEMBLER assembler = FASM;

    // compile
    std::cout << INFO_COLOR << "[info] generating asm from '" << argv[1] << "'." << RESET_COLOR << std::endl;
    compile_to_asm(program, "out.asm", assembler);

    // nasm or fasm
    std::cout << INFO_COLOR << "[info] compiling assembly." << RESET_COLOR << std::endl;

    if (assembler == FASM)
    {
        exit_on_error(std::system("fasm out.asm"));
    }
    else
    {
        exit_on_error(std::system("nasm -felf64 out.asm"));

        // link
        std::cout << INFO_COLOR << "[info] linking object file." << RESET_COLOR << std::endl;
        exit_on_error(std::system("ld -o out out.o"));
    
        // cleanup
        std::cout << INFO_COLOR << "[info] cleaning up object file" << RESET_COLOR << std::endl;
        exit_on_error(std::system("rm out.o"));
    }

    return 0;
}
