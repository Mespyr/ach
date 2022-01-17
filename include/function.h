#ifndef ILU_FUNCTION_H
#define ILU_FUNCTION_H

#include "ops.h"
#include "datatype.h"
#include <vector>

class Function
{
public:
    Function(Op op, std::vector<Op> ops, std::vector<IluTypeWithOp> arg_stack, std::vector<IluTypeWithOp> ret_stack) :
        op(op),
        ops(ops),
        arg_stack(arg_stack),
        ret_stack(ret_stack)
    {}

    Op op;
    std::vector<Op> ops;
    std::vector<IluTypeWithOp> arg_stack;
    std::vector<IluTypeWithOp> ret_stack;
};


#endif
