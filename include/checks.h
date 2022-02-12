#ifndef _ACH_CHECKS_H_
#define _ACH_CHECKS_H_

#include "error.h"
#include "ops.h"
#include "datatype.h"
#include "program.h"
#include "error.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

class StackSnapshot
{
public:
	StackSnapshot(std::vector<TypeAtLoc> type_stack, OpType type) :
		type_stack(type_stack),
		type(type)
	{}

	std::vector<TypeAtLoc> type_stack;
	OpType type;
};

bool compare_type_stacks(std::vector<TypeAtLoc> type_stack_1, std::vector<TypeAtLoc> type_stack_2);
void verify_program(Program program);
void type_check_program(Program program);

#endif
