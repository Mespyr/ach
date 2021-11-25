#include "../include/ops.h"
#include "../include/compiler.h"

#include <cstdlib>
#include <vector>

int main(int argc, char* argv[]) 
{
    std::vector<Op> program = {
        push(34),
        push(35),
        plus(),
        dump(),
        push(500),
        push(80),
        minus(),
        dump()
    };

    compile_to_asm(program, "out.asm");

    std::system("nasm -felf64 out.asm");
    std::system("ld out.o");

    return 0;
}
