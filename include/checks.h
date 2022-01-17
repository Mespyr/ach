#ifndef ILU_CHECKS_H
#define ILU_CHECKS_H

#include "error.h"
#include "ops.h"
#include "datatype.h"
#include "function.h"
#include "error.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>

class StackSnapshot
{
public:
    StackSnapshot(std::vector<IluTypeWithOp> type_stack, OpType type) :
        type_stack(type_stack),
        type(type)
    {}

    std::vector<IluTypeWithOp> type_stack;
    OpType type;
};

bool compare_type_stacks(std::vector<IluTypeWithOp> type_stack_1, std::vector<IluTypeWithOp> type_stack_2);
void type_check_program(std::map<std::string, Function> program);

#endif
