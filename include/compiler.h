#ifndef ILU_COMPILER_H
#define ILU_COMPILER_H

#include "ops.h"
#include "file.h"
#include "error.h"
#include <vector>
#include <string>
#include <ios>
#include <sstream>
#include <map>

void compile_to_asm(std::map<std::string, std::vector<Op>> program, std::string output_filename);

#endif
