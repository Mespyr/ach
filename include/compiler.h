#ifndef ILU_COMPILER_H
#define ILU_COMPILER_H

#include "ops.h"
#include "file.h"
#include "error.h"
#include "program.h"
#include <vector>
#include <string>
#include <ios>
#include <sstream>
#include <map>
#include <assert.h>

enum ASSEMBLER
{
    NASM,
    FASM,

    // count | must always be last
    ASSEMBLER_COUNT
};

void compile_to_asm(Program program, std::string output_filename, ASSEMBLER assembler);

#endif
