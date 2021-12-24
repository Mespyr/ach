#ifndef ILU_TOKEN_H
#define ILU_TOKEN_H

#include "location.h"
#include <string>

class Token
{
public: 
    Token(std::string value, Location loc) :
        value(value),
        loc(loc)
    {}

    std::string value;
    Location loc;
};

#endif
