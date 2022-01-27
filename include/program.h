#ifndef ILU_PROGRAM_H
#define ILU_PROGRAM_H

#include "ops.h"
#include "datatype.h"
#include <map>
#include <string>

class Const
{
public:
    Const(Op op, long long value) :
        op(op),
        value(value)
    {}

    Op op;
    long long value;
};

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

class Program 
{
public:
    std::map<std::string, Function> functions;
    std::map<std::string, Const> consts;
    std::map<std::string, long unsigned int> memories;

    long unsigned int memory_capacity = 0;
};

#endif
