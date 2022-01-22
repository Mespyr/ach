#ifndef ILU_FUNCTION_H
#define ILU_FUNCTION_H

#include "ops.h"
#include "datatype.h"
#include <vector>

class Function
{
public:
    Function(Op op, std::vector<IluTypeWithOp> arg_stack, std::vector<IluTypeWithOp> ret_stack, int addr) :
        op(op),
        arg_stack(arg_stack),
        ret_stack(ret_stack),
        addr(addr)
    {}

    Op op;
    std::vector<Op> ops;
    std::vector<IluTypeWithOp> arg_stack;
    std::vector<IluTypeWithOp> ret_stack;
    int addr;
};

#endif
