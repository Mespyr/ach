#include <vector>

#include "../include/ops.h"
#include "../include/compiler.h"

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

    compile(program);

    return 0;
}
