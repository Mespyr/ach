#include "../include/ops.h"
#include "../include/compiler.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include <cstdlib>
#include <vector>
#include <iostream>

void usage(char* exec_name)
{
    std::cout << "[usage] " << exec_name << " <filename>" << std::endl;
}

int main(int argc, char* argv[]) 
{
    if (argc == 1)
    {
        usage(argv[0]);
        std::cerr << "[error] no file provided for compiler" << std::endl;
        exit(1);
    }

    File f(argv[1], MODE_READ);
    std::string code = f.read();

    std::vector<std::string> tokens = tokenize(code);
    std::vector<Op> program = parse_tokens(tokens);

    std::cout << "[info] compiling to out.asm" << std::endl;
    compile_to_asm(program, "out.asm");
    
    std::cout << "[info] generating object file" << std::endl;
    int return_code = std::system("nasm -felf64 out.asm");
    if (return_code != 0) exit(1);

    std::cout << "[info] linking to ./a.out" << std::endl;
    return_code = std::system("ld out.o");
    if (return_code != 0) exit(1);

    std::cout << "[info] cleaning up" << std::endl;
    std::system("rm out.o");

    return 0;
}
