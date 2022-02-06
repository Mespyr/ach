#include "../include/error.h"

void print_error(std::string message)
{
	std::cerr << "ERROR: " << message << std::endl;
}

void print_error_at_loc(Location loc, std::string message)
{
	std::cerr << loc.file_location << ":" << loc.line_number << ":" << loc.column_number_start << ": " << message << std::endl;
	std::cerr << loc.line << std::endl;

	for (long unsigned int i = 0; i < loc.column_number_start; i++)
	{
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << " ";
	}

	for (long unsigned int i = loc.column_number_start; i < loc.column_number_end; i++)
	{
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << "^";
	}

	std::cerr << std::endl;
}

void print_note_at_loc(Location loc, std::string message)
{
	std::cerr << loc.file_location << ":" << loc.line_number << ":" << loc.column_number_start << ": note: " << message << std::endl;
	std::cerr << loc.line << std::endl;

	for (long unsigned int i = 0; i < loc.column_number_start; i++)
	{
		if (loc.line.at(i) == '\t')
			std::cerr << "\t";
		else std::cerr << " ";
	}

	for (long unsigned int i = loc.column_number_start; i < loc.column_number_end; i++)
	{
		if (loc.line.at(i) == '\t')
			std::cerr << "^^^^";
		else std::cerr << "^";
	}

	std::cerr << std::endl;
}

void print_not_enough_arguments_error(Location loc, int expected, int actual, std::string name, std::string fullname, bool is_block, bool is_func)
{
	if (fullname == "")
	{
		if (is_block)
			// not enough items on stack for 'name' block (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for '" + name + "' block (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
		else if (is_func)
			// not enough items on stack for function 'name' (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for function '" + name + "' (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
		else
			// not enough items on stack for 'name' operation (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for '" + name + "' operation (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
	}
	else
	{
		if (is_block)
			// not enough items on stack for 'name' (fullname) block (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for '" + name + "' (" + fullname + ") block (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
		else if (is_func)
			// not enough items on stack for function 'name' (fullname) (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for function '" + name + "' (" + fullname + ") (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
		else
			// not enough items on stack for 'name' (fullname) operation (expected 'expected', got 'actual')
			print_error_at_loc(loc, "not enough items on stack for '" + name + "' (" + fullname + ") operation (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
	}
}

void print_invalid_combination_of_types_error(Location loc, std::vector<DATATYPE> types, std::string name, std::string fullname, bool is_block, bool is_func_ret, bool is_func_args)
{
	// ex: 'int, int, ptr, int'
	std::string types_str;
	for (unsigned long int i = 0; i < types.size() - 1; i++)
		types_str.append(human_readable_type(types.at(i)) + ", ");
	types_str.append(human_readable_type(types.back()));

	if (fullname == "")
	{
		if (is_block)
			// invalid combination of types for 'name' condition (types_str)
			print_error_at_loc(loc, "invalid combination of types for '" + name + "' condition (" + types_str + ")");
		else if (is_func_ret)
			// invalid combination of types for function 'name' return stack (types_str)
			print_error_at_loc(loc, "invalid combination of types for function '" + name + "' return stack (" + types_str + ")");
		else if (is_func_args)
			// invalid combination of types for function 'name' arguments (types_str)
			print_error_at_loc(loc, "invalid combination of types for function '" + name + "' arguments (" + types_str + ")");
		else
			// invalid combination of types for 'name' operation (types_str)
			print_error_at_loc(loc, "invalid combination of types for '" + name + "' operation (" + types_str + ")");
	}
	else
	{
		if (is_block)
			// invalid combination of types for 'name' (fullname) condition (types_str)
			print_error_at_loc(loc, "invalid combination of types for '" + name + "' (" + fullname + ") condition (" + types_str + ")");
		else if (is_func_ret)
			// invalid combination of types for function 'name' (fullname) return stack (types_str)
			print_error_at_loc(loc, "invalid combination of types for function '" + name + "' (" + fullname + ") return stack (" + types_str + ")");
		else if (is_func_args)
			// invalid combination of types for function 'name' (fullname) arguments (types_str)
			print_error_at_loc(loc, "invalid combination of types for function '" + name + "' (" + fullname + ") arguments (" + types_str + ")");
		else
			// invalid combination of types for 'name' (fullname) operation (types_str)
			print_error_at_loc(loc, "invalid combination of types for '" + name + "' (" + fullname + ") operation (" + types_str + ")");
	}
}

void print_invalid_type_error(Location loc, DATATYPE expected, DATATYPE actual, std::string name, std::string fullname, bool is_block, bool is_func_ret, bool is_func_args)
{
	if (fullname == "")
	{
		if (is_block)
			// invalid argument type for 'name' condition (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid type for '" + name + "' condition (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
		else if (is_func_ret)
			// invalid return type for function 'name' (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid return type for function '" + name + "' (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
		else if (is_func_args)
			// invalid argument type for function 'name' (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid argument type for function '" + name + "' (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
		else
			// invalid argument type for 'name' operation (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid type for '" + name + "' operation (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
	}
	else
	{
		if (is_block)
			// invalid argument type for 'name' (fullname) condition (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid type for '" + name + "' (" + fullname + ") condition (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
		else if (is_func_ret)
			// invalid return type for function 'name' (fullname) (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid return type for function '" + name + "' (" + fullname + ") (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
		else if (is_func_args)
			// invalid argument type for function 'name' (fullname) (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid argument type for function '" + name + "' (" + fullname + ") (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
		else
			// invalid argument type for 'name' (fullname) operation (expected 'expected', got 'actual')
			print_error_at_loc(loc, "invalid type for '" + name + "' (" + fullname + ") operation (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
	}
}
