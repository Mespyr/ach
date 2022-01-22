#ifndef ILU_PROGRAM_H
#define ILU_PROGRAM_H

#include "const.h"
#include "function.h"
#include <map>
#include <string>

class Program 
{
public:
    std::map<std::string, Function> functions;
    std::map<std::string, Const> consts;
};

#endif
