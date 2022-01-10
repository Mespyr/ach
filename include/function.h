#ifndef ILU_FUNCTION_H
#define ILU_FUNCTION_H

#include "ops.h"
#include <vector>

class Function
{
public:
    Function(std::vector<Op> ops) :
        ops(ops)
    {}

    std::vector<Op> ops;
};

#endif
