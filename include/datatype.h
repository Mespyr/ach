#ifndef ILU_DATATYPE_H
#define ILU_DATATYPE_H

#include "location.h"
#include <string>

enum DATATYPE
{
    DATATYPE_INT,
    DATATYPE_PTR,

    // count
    DATATYPE_COUNT
};

class TypeAtLoc
{
public:
    TypeAtLoc(Location loc, DATATYPE type) :
        loc(loc),
        type(type) 
    {}

    Location loc;
    DATATYPE type;
};

std::string human_readable_type(DATATYPE type);

#endif
