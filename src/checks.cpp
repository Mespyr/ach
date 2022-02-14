#include "../include/checks.h"

bool compare_type_stacks(std::vector<TypeAtLoc> type_stack_1, std::vector<TypeAtLoc> type_stack_2)
{
	if (type_stack_1.size() != type_stack_2.size())
		return false;

	for (long unsigned int i = 0; i < type_stack_1.size(); i++)
	{
		DATATYPE type_1 = type_stack_1.at(i).type;
		DATATYPE type_2 = type_stack_2.at(i).type;
		if (type_1 != type_2)
			return false;
	}

	return true;
}

void verify_program(Program program)
{
	if (!program.functions.count("main"))
	{
		print_error("no entry point found in program (no 'main' function)");
		exit(1);
	}

	Function main_func = program.functions.at("main");
	if (main_func.arg_stack.size() > 0)
	{
		print_error_at_loc(main_func.loc, "'main' function must not pass any arguments");
		exit(1);
	}

	if (main_func.ret_stack.size() > 0)
	{
		print_error_at_loc(main_func.loc, "'main' function must not have any return values");
		exit(1);
	}
}

void type_check_program(Program program)
{
	static_assert(OP_COUNT == 59, "unhandled op types in type_check_program()");
	static_assert(DATATYPE_COUNT == 2, "unhandled datatypes in type_check_program()");

	for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++)
	{
		std::vector<TypeAtLoc> type_stack;
		std::vector<StackSnapshot> stack_snapshots;
		std::vector<std::vector<TypeAtLoc>> let_bindings_stack;
		Function function = fn_key->second;

		for (TypeAtLoc t : function.arg_stack)
			type_stack.push_back(t);

		std::string func_name = fn_key->first;

		for (Op op : function.ops)
		{
			// debugging
			if (op.type == OP_DUMP)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "dump");
					exit(1);
				}
				type_stack.pop_back();
			}

			// arithmetics
			else if (op.type == OP_PLUS)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "+", "addition");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// additions goes in following combinations [b, a] -> [b + a]

				// int + int -> int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				// ptr + int -> ptr
				else if (a.type == DATATYPE_INT && b.type == DATATYPE_PTR)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
				// int + ptr -> ptr
				else if (a.type == DATATYPE_PTR && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "+", "addition");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				} }
			else if (op.type == OP_MINUS)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "-", "subtraction");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// subtraction goes in following combinations [b, a] -> [b - a]

				// int - int -> int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				// ptr - int -> ptr
				else if (a.type == DATATYPE_INT && b.type == DATATYPE_PTR)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
				// ptr - ptr -> int
				else if (a.type == DATATYPE_PTR && b.type == DATATYPE_PTR)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "-", "subtraction");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_MUL)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "*", "multiplication");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// multiplication goes in following combinations

				// int * int -> int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "*", "multiplication");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_DIV)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "/", "division");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// division goes in following combinations

				// int / int -> int, int
				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
				{
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				}
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "/", "division");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			
			// comparisons
			else if (op.type == OP_EQUAL)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "=", "equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "=", "equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_GREATER)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), ">", "greater than");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, ">", "greater than");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_LESS)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "<", "less than");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "<", "less than");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_GREATER_EQ)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), ">=", "greater than or equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, ">=", "greater than or equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_LESS_EQ)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "<=", "less than or equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, ">=", "less than or equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_NOT_EQ)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "!=", "not equal to");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "!=", "not equal to");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_NOT)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "not");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "not");
					print_note_at_loc(a.loc, "first argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_AND)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "and");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "and");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_OR)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "or");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == b.type)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "or");
					print_note_at_loc(b.loc, "first argument found here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second argument found here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			
			// stack manipulation
			else if (op.type == OP_POP)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "pop");
					exit(1);
				}
				type_stack.pop_back();
			}
			else if (op.type == OP_DUP)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "dup");
					exit(1);
				}
				TypeAtLoc a = type_stack.back();
				a.loc = op.loc;
				type_stack.push_back(a);
			}
			else if (op.type == OP_SWP)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "swp");
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();
				type_stack.push_back(a);
				type_stack.push_back(b);
			}
			else if (op.type == OP_ROT)
			{
				if (type_stack.size() < 3)
				{
					print_not_enough_arguments_error(op.loc, 3, type_stack.size(), "rot", "rotate");
					exit(1);
				}

				// [c, b, a] -> [b, a, c]
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();
				TypeAtLoc c = type_stack.back(); type_stack.pop_back();
				type_stack.push_back(b);
				type_stack.push_back(a);
				type_stack.push_back(c);
			}
			else if (op.type == OP_OVER)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "over");
					exit(1);
				}
				
				// [b, a] -> [b, a, b]
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();
				type_stack.push_back(b);
				type_stack.push_back(a);
				b.loc = op.loc;
				type_stack.push_back(b);
			}
		   
			// memory
			else if (op.type == OP_READ8)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "@8", "8-bit read");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				if (a.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, a.type, "@8", "8-bit read");
					print_note_at_loc(a.loc, "first value pushed here");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_WRITE8)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "!8", "8-bit write");
					exit(1);
				}
				 
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// [ptr, any] or [b, a] -> []
				if (b.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, b.type, "!8", "8-bit write");
					print_note_at_loc(b.loc, "argument pushed here (" + human_readable_type(b.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_READ16)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "@16", "16-bit read");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				if (a.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, a.type, "@16", "16-bit read");
					print_note_at_loc(a.loc, "first value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_WRITE16)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "!16", "16-bit write");
					exit(1);
				}
				 
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// [ptr, any] or [b, a] -> []
				if (b.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, b.type, "!16", "16-bit write");
					print_note_at_loc(b.loc, "argument pushed here (" + human_readable_type(b.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_READ32)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "@32", "32-bit read");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				if (a.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, a.type, "@32", "32-bit read");
					print_note_at_loc(a.loc, "first value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_WRITE32)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "!32", "32-bit write");
					exit(1);
				}
				 
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();
				
				// [ptr, any] or [b, a] -> []
				if (b.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, b.type, "!32", "32-bit write");
					print_note_at_loc(b.loc, "argument pushed here (" + human_readable_type(b.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_READ64)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "@64", "64-bit read");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				if (a.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, a.type, "@64", "64-bit read");
					print_note_at_loc(a.loc, "first value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_WRITE64)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "!64", "64-bit write");
					exit(1);
				}
				 
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				// [ptr, any] or [b, a] -> []
				if (b.type != DATATYPE_PTR)
				{
					print_invalid_type_error(op.loc, DATATYPE_PTR, b.type, "!64", "64-bit write");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					exit(1);
				}
			}

			// argv
			else if (op.type == OP_ARGV)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
			}
			else if (op.type == OP_ARGC)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}

			// bitwise
			else if (op.type == OP_SHIFT_LEFT)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "<<", "shift left");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, "<<", "shift left");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}
			else if (op.type == OP_SHIFT_RIGHT)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), ">>", "shift right");
					exit(1);
				}
				
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				TypeAtLoc b = type_stack.back(); type_stack.pop_back();

				if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
					type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				else
				{
					print_invalid_combination_of_types_error(op.loc, {b.type, a.type}, ">>", "shift right");
					print_note_at_loc(b.loc, "first value pushed here (" + human_readable_type(b.type) + ")");
					print_note_at_loc(a.loc, "second value pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
			}

			// syscalls
			else if (op.type == OP_SYSCALL0)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "syscall0");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall0");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL1)
			{
				if (type_stack.size() < 2)
				{
					print_not_enough_arguments_error(op.loc, 2, type_stack.size(), "syscall1");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall1");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL2)
			{
				if (type_stack.size() < 3)
				{
					print_not_enough_arguments_error(op.loc, 3, type_stack.size(), "syscall2");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall2");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL3)
			{
				if (type_stack.size() < 4)
				{
					print_not_enough_arguments_error(op.loc, 4, type_stack.size(), "syscall3");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall3");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL4)
			{
				if (type_stack.size() < 5)
				{
					print_not_enough_arguments_error(op.loc, 5, type_stack.size(), "syscall4");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall4");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL5)
			{
				if (type_stack.size() < 6)
				{
					print_not_enough_arguments_error(op.loc, 6, type_stack.size(), "syscall5");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall5");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_SYSCALL6)
			{
				if (type_stack.size() < 7)
				{
					print_not_enough_arguments_error(op.loc, 7, type_stack.size(), "syscall6");
					exit(1);
				}
				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();
				type_stack.pop_back();

				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "syscall6");
					print_note_at_loc(a.loc, "syscall number pushed here (" + human_readable_type(a.type) + ")");
					exit(1);
				}
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}

			// keywords
			else if (op.type == OP_WHILE)
			{
				stack_snapshots.push_back(StackSnapshot(type_stack, op.type));
			}
			else if (op.type == OP_DO)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "do", "while loop", true);
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "do", "while loop", true);
					print_note_at_loc(a.loc, "first value pushed here");
					exit(1);
				}
				
				StackSnapshot snapshot = stack_snapshots.back(); stack_snapshots.pop_back();
				bool type_stack_equal = compare_type_stacks(snapshot.type_stack, type_stack);

				if (!type_stack_equal)
				{
					print_error_at_loc(op.loc, "while loops are not allowed to change the types and amount of items on stack in the condition");
					// TODO: add notes for state of stack
					exit(1);
				}
				stack_snapshots.push_back(StackSnapshot(type_stack, op.type));
			}
			else if (op.type == OP_IF)
			{
				if (type_stack.size() < 1)
				{
					print_not_enough_arguments_error(op.loc, 1, 0, "if", "", true);
					exit(1);
				}

				TypeAtLoc a = type_stack.back(); type_stack.pop_back();
				if (a.type != DATATYPE_INT)
				{
					print_invalid_type_error(op.loc, DATATYPE_INT, a.type, "if", "", true);
					print_note_at_loc(a.loc, "first value pushed here");
					exit(1);
				}
				stack_snapshots.push_back(StackSnapshot(type_stack, op.type));
			}
			else if (op.type == OP_ELSE)
			{
				StackSnapshot snapshot = stack_snapshots.back(); stack_snapshots.pop_back();
				stack_snapshots.push_back(StackSnapshot(type_stack, op.type));
				type_stack = snapshot.type_stack;
			}
			else if (op.type == OP_LET)
			{
				stack_snapshots.push_back(StackSnapshot({}, op.type));

				if ((long long) type_stack.size() < op.int_operand)
				{
					print_not_enough_arguments_error(op.loc, op.int_operand, type_stack.size(), "let", "let binding", true);
					exit(1);
				}

				std::vector<TypeAtLoc> vars;
				for (int a = 0; a < op.int_operand; a++)
				{
					vars.push_back(type_stack.back());
					type_stack.pop_back();
				}
				std::reverse(vars.begin(), vars.end());
				if (let_bindings_stack.size() > 0)
				{
					std::vector<TypeAtLoc> all_vars = let_bindings_stack.back();
					for (TypeAtLoc t : vars)
						all_vars.push_back(t);
					let_bindings_stack.push_back(all_vars);
				}
				else let_bindings_stack.push_back(vars);
			}
			else if (op.type == OP_END)
			{
				StackSnapshot snapshot = stack_snapshots.back(); stack_snapshots.pop_back();

				if (snapshot.type == OP_LET)
				{
					assert(let_bindings_stack.size() > 0);
					let_bindings_stack.pop_back();
				}
				else if (snapshot.type == OP_IF)
				{
					bool type_stack_equal = compare_type_stacks(snapshot.type_stack, type_stack);
					if (!type_stack_equal)
					{
						print_error_at_loc(op.loc, "if statements are not allowed to change the types of items on stack");
						// TODO: add notes for state of stack
						exit(1);
					}
				}
				else if (snapshot.type == OP_ELSE)
				{
					bool type_stack_equal = compare_type_stacks(snapshot.type_stack, type_stack);
					if (!type_stack_equal)
					{
						print_error_at_loc(op.loc, "both branches of if-else statements must produce the same types on the stack");
						// TODO: add notes for state of stack
						exit(1);
					}
				}
				else if (snapshot.type == OP_DO)
				{
					bool type_stack_equal = compare_type_stacks(snapshot.type_stack, type_stack);
					if (!type_stack_equal)
					{
						print_error_at_loc(op.loc, "while loop are not allowed to change the types of the items on the stack");
						// TODO: add notes for state of stack
						exit(1);
					}
				}
			}

			// type checking
			else if (op.type == OP_CAST_PTR)
			{
				type_stack.pop_back();
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
			}
			else if (op.type == OP_CAST_INT)
			{
				type_stack.pop_back();
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}

			// other
			else if (op.type == OP_PUSH_INT)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
			}
			else if (op.type == OP_PUSH_STR)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_INT));
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
			}
			else if (op.type == OP_PUSH_CSTR)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
			}
			else if (op.type == OP_FUNCTION_CALL)
			{
				assert(program.functions.count(op.str_operand));

				Function call_func = program.functions.at(op.str_operand);

				if (type_stack.size() < call_func.arg_stack.size())
				{
					print_not_enough_arguments_error(op.loc, call_func.arg_stack.size(), type_stack.size(), op.str_operand, "", false, true);
					exit(1);
				}

				std::vector<TypeAtLoc> args;
				for (unsigned long int i = call_func.arg_stack.size(); i > 0; i--)
				{
					args.push_back(type_stack.back());
					type_stack.pop_back();
				}
				std::reverse(args.begin(), args.end());

				bool args_match_types = compare_type_stacks(args, call_func.arg_stack);
				if (!args_match_types)
				{
					if (call_func.arg_stack.size() == 1)
						print_invalid_type_error(op.loc, call_func.arg_stack.at(0).type, args.at(0).type, op.str_operand, "", false, false, true);
					else
					{
						std::vector<DATATYPE> types;
						for (TypeAtLoc t : args)
							types.push_back(t.type);

						print_invalid_combination_of_types_error(op.loc, types, op.str_operand, "", false, false, true);
						for (TypeAtLoc t : args)
							print_note_at_loc(t.loc, "argument pushed here (" + human_readable_type(t.type) + ")");
					}
					exit(1);
				}

				for (TypeAtLoc t : call_func.ret_stack)
					type_stack.push_back(t);
			}
			else if (op.type == OP_PUSH_GLOBAL_MEM)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
			}
			else if (op.type == OP_PUSH_LOCAL_MEM)
			{
				type_stack.push_back(TypeAtLoc(op.loc, DATATYPE_PTR));
			}
			else if (op.type == OP_PUSH_LET_BOUND_VAR)
			{
				assert(let_bindings_stack.size() > 0);
				type_stack.push_back(let_bindings_stack.back().at(op.int_operand));
			}

			// unreachable
			else if (op.type == OP_OFFSET || op.type == OP_RESET || op.type == OP_DEF || op.type == OP_CONST || op.type == OP_MEMORY)
			{
				print_error_at_loc(op.loc, "unreachable: op should be handled in the parsing step. This is probably a bug.");
				exit(1);
			}
		}

		bool return_vals_equal = compare_type_stacks(type_stack, function.ret_stack);
		if (!return_vals_equal)
		{
			if (function.ret_stack.size() < type_stack.size()) // main cannot have excess data on stack
			{
				print_error_at_loc(function.loc, "unhandled data on the stack (expected " + std::to_string(function.ret_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

				std::vector<TypeAtLoc> excess_stack;
				for (unsigned long int i = type_stack.size() - function.ret_stack.size(); i > 0; i--)
				{
					excess_stack.push_back(type_stack.back());
					type_stack.pop_back();
				}
				std::reverse(excess_stack.begin(), excess_stack.end());

				for (TypeAtLoc t : excess_stack)
					print_note_at_loc(t.loc, "excess data pushed here (" + human_readable_type(t.type) + ")");
			}

			else if (function.ret_stack.size() > type_stack.size())
				print_error_at_loc(function.loc, "not enough data on the stack (expected " + std::to_string(function.ret_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

			else if (type_stack.size() > 1)
			{
				std::vector<DATATYPE> types;
				for (TypeAtLoc t : type_stack)
					types.push_back(t.type);

				print_invalid_combination_of_types_error(function.loc, types, func_name, "", false, true);
				for (TypeAtLoc t : type_stack)
					print_note_at_loc(t.loc, "argument pushed here (" + human_readable_type(t.type) + ")");
			}

			else
			{
				print_invalid_type_error(function.loc, function.ret_stack.at(0).type, type_stack.at(0).type, func_name, "", false, true);
				print_note_at_loc(type_stack.at(0).loc, "value pushed here (" + human_readable_type(type_stack.at(0).type) + ")");
			}

			exit(1);
		}
	}
}
