#ifndef ILU_CONST_H
#define ILU_CONST_H

#include "ops.h"
#include "datatype.h"
#include <vector>
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

#endif
