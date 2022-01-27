#ifndef ILU_DATATYPE_H
#define ILU_DATATYPE_H

#include "ops.h"
#include <string>

enum DATATYPE
{
    DATATYPE_INT,
    DATATYPE_PTR,

    // count
    DATATYPE_COUNT
};

class IluTypeWithOp
{
public:
    IluTypeWithOp(Op op, DATATYPE type) :
        op(op),
        type(type) 
    {}

    Op op;
    DATATYPE type;
};

std::string human_readable_type(DATATYPE type);

#endif
