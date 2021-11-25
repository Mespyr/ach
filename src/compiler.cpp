#include "../include/compiler.h"
#include <iostream>

void compile(std::vector<Op> program)
{
    std::vector<int> stack;

    for (Op op : program)
    {
        if (op.type == OP_PUSH)
            stack.push_back(op.n);
        else if (op.type == OP_DUMP)
        {
            std::cout << stack.back() << std::endl;
            stack.pop_back();
        }
        else if (op.type == OP_PLUS)
        {
            int a = stack.back(); stack.pop_back();
            int b = stack.back(); stack.pop_back();
            stack.push_back(a + b);
        }
        else if (op.type == OP_MINUS)
        {
            int a = stack.back(); stack.pop_back();
            int b = stack.back(); stack.pop_back();
            stack.push_back(b - a);
        }
    }
}
