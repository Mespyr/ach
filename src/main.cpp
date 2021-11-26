#include "../include/ops.h"
#include "../include/compiler.h"

#include <cstdlib>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) 
{
    std::vector<Op> program = {
        push(34),
        push(35),
        plus(),
        print(),
        push(500),
        push(80),
        minus(),
        print(),
        push(80),
        push(500),
        minus(),
        print()
    };

    std::cout << "[info] compiling to out.asm" << std::endl;
    compile_to_asm(program, "out.asm");
    
    std::cout << "[info] generating object file" << std::endl;
    int return_code = std::system("nasm -felf64 out.asm");
    if (return_code != 0) exit(1);

    std::cout << "[info] linking to ./a.out" << std::endl;
    return_code = std::system("ld out.o");
    if (return_code != 0) exit(1);

    return 0;
}
