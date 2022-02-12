#ifndef _ACH_PROGRAM_H_
#define _ACH_PROGRAM_H_

#include "location.h"
#include "datatype.h"
#include "ops.h"
#include <map>
#include <string>

class Const
{
public:
	Const(Location loc, long long value) :
		loc(loc),
		value(value)
	{}

	Location loc;
	long long value;
};

class Memory
{
public:
	Memory(Location loc, long unsigned int offset) :
		loc(loc),
		offset(offset)
	{}

	Location loc;
	long unsigned int offset;
};

class Function
{
public:
	Function(Location loc, std::vector<TypeAtLoc> arg_stack, std::vector<TypeAtLoc> ret_stack, int addr) :
		loc(loc),
		arg_stack(arg_stack),
		ret_stack(ret_stack),
		addr(addr)
	{}

	Location loc;
	std::vector<Op> ops;
	std::vector<TypeAtLoc> arg_stack;
	std::vector<TypeAtLoc> ret_stack;
	int addr;

	std::map<std::string, Memory> memories;
	long unsigned int memory_capacity = 0;
};

class Program 
{
public:
	std::map<std::string, Function> functions;
	std::map<std::string, Const> consts;
	std::map<std::string, Memory> memories;
	long unsigned int memory_capacity = 0;
};

#endif
