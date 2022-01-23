#include "../include/checks.h"

bool compare_type_stacks(std::vector<IluTypeWithOp> type_stack_1, std::vector<IluTypeWithOp> type_stack_2)
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
        print_op_error(main_func.op, "'main' function must not pass any arguments");
        exit(1);
    }

    if (main_func.ret_stack.size() > 0)
    {
        print_op_error(main_func.op, "'main' function must not have any return values");
        exit(1);
    }
}

void type_check_program(Program program)
{
    static_assert(OP_COUNT == 53, "unhandled op types in type_check_program()");
    static_assert(DATATYPE_COUNT == 2, "unhandled datatypes in type_check_program()");

    for (auto fn_key = program.functions.begin(); fn_key != program.functions.end(); fn_key++)
    {
        std::vector<IluTypeWithOp> type_stack;
        std::vector<StackSnapshot> stack_snapshots;
        Function function = fn_key->second;

        for (IluTypeWithOp t : function.arg_stack)
            type_stack.push_back(t);

        std::string func_name = fn_key->first;

        for (Op op : function.ops)
        {
            // debugging
            if (op.type == OP_DUMP)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "dump");
                    exit(1);
                }
                type_stack.pop_back();
            }

            // arithmetics
            else if (op.type == OP_PLUS)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "+", "addition");
                    exit(1);
                }

                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                // additions goes in following combinations [b, a] -> [b + a]

                // int + int -> int
                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                // ptr + int -> ptr
                else if (a.type == DATATYPE_INT && b.type == DATATYPE_PTR)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
                // int + ptr -> ptr
                else if (a.type == DATATYPE_PTR && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "+", "addition");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                } }
            else if (op.type == OP_MINUS)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "-", "subtraction");
                    exit(1);
                }

                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                // subtraction goes in following combinations [b, a] -> [b - a]

                // int - int -> int
                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                // ptr - int -> ptr
                else if (a.type == DATATYPE_INT && b.type == DATATYPE_PTR)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
                // ptr - ptr -> int
                else if (a.type == DATATYPE_PTR && b.type == DATATYPE_PTR)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "-", "subtraction");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_MUL)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "*", "multiplication");
                    exit(1);
                }

                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                // multiplication goes in following combinations

                // int * int -> int
                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "*", "multiplication");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_DIV)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "/", "division");
                    exit(1);
                }

                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                // division goes in following combinations

                // int / int -> int, int
                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                {
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                }
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "/", "division");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            
            // comparisons
            else if (op.type == OP_EQUAL)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "=", "equal to");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "=", "equal to");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_GREATER)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), ">", "greater than");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, ">", "greater than");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_LESS)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "<", "less than");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "<", "less than");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_GREATER_EQ)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), ">=", "greater than or equal to");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, ">=", "greater than or equal to");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_LESS_EQ)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "<=", "less than or equal to");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, ">=", "less than or equal to");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_NOT_EQ)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "!=", "not equal to");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "!=", "not equal to");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_NOT)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "not");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();

                if (a.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "not");
                    print_op_note(a.op, "first argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_AND)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "and");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "and");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_OR)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "or");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == b.type)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "or");
                    print_op_note(b.op, "first argument found here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second argument found here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            
            // stack manipulation
            else if (op.type == OP_POP)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "pop");
                    exit(1);
                }
                type_stack.pop_back();
            }
            else if (op.type == OP_DUP)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "dup");
                    exit(1);
                }
                IluTypeWithOp a = type_stack.back();
                a.op = op;
                type_stack.push_back(a);
            }
            else if (op.type == OP_SWP)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "swp");
                    exit(1);
                }

                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();
                type_stack.push_back(a);
                type_stack.push_back(b);
            }
            else if (op.type == OP_ROT)
            {
                if (type_stack.size() < 3)
                {
                    print_not_enough_arguments_error(op, 3, type_stack.size(), "rot", "rotate");
                    exit(1);
                }

                // [c, b, a] -> [b, a, c]
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp c = type_stack.back(); type_stack.pop_back();
                type_stack.push_back(b);
                type_stack.push_back(a);
                type_stack.push_back(c);
            }
            else if (op.type == OP_OVER)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "over");
                    exit(1);
                }
                
                // [b, a] -> [b, a, b]
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();
                type_stack.push_back(b);
                type_stack.push_back(a);
                b.op = op;
                type_stack.push_back(b);
            }
           
            // memory
            else if (op.type == OP_MEM)
            {
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
            }
            else if (op.type == OP_READ8)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "read8");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                if (a.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, a.type, "read8");
                    print_op_note(op, "first value pushed here");
                    exit(1);
                }
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
            }
            else if (op.type == OP_WRITE8)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "write8");
                    exit(1);
                }
                 
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                // [ptr, any] or [b, a] -> []
                if (b.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, b.type, "write8");
                    print_op_note(b.op, "argument pushed here (" + human_readable_type(b.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_READ16)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "read16");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                if (a.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, a.type, "read16");
                    print_op_note(op, "first value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
            }
            else if (op.type == OP_WRITE16)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "write16");
                    exit(1);
                }
                 
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                // [ptr, any] or [b, a] -> []
                if (b.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, b.type, "write16");
                    print_op_note(b.op, "argument pushed here (" + human_readable_type(b.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_READ32)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "read32");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                if (a.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, a.type, "read32");
                    print_op_note(op, "first value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
            }
            else if (op.type == OP_WRITE32)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "write32");
                    exit(1);
                }
                 
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();
                
                // [ptr, any] or [b, a] -> []
                if (b.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, b.type, "write32");
                    print_op_note(b.op, "argument pushed here (" + human_readable_type(b.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_READ64)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "read64");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                if (a.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, a.type, "read64");
                    print_op_note(op, "first value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
            }
            else if (op.type == OP_WRITE64)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "write64");
                    exit(1);
                }
                 
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                // [ptr, any] or [b, a] -> []
                if (b.type != DATATYPE_PTR)
                {
                    print_invalid_type_error(op, DATATYPE_PTR, b.type, "write64");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    exit(1);
                }
            }

            // argv
            else if (op.type == OP_ARGV)
            {
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
            }
            else if (op.type == OP_ARGC)
            {
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
            }

            // bitwise
            else if (op.type == OP_SHIFT_LEFT)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "<<", "shift left");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "<<", "shift left");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_SHIFT_RIGHT)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), ">>", "shift right");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, ">>", "shift right");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_ORB)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "orb", "bitwise or");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "orb", "bitwise or");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_ANDB)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "andb", "bitwise and");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                IluTypeWithOp b = type_stack.back(); type_stack.pop_back();

                if (a.type == DATATYPE_INT && b.type == DATATYPE_INT)
                    type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                else
                {
                    print_invalid_combination_of_types_error(op, {b.type, a.type}, "andb", "bitwise and");
                    print_op_note(b.op, "first value pushed here (" + human_readable_type(b.type) + ")");
                    print_op_note(a.op, "second value pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            
            // syscalls
            else if (op.type == OP_SYSCALL1)
            {
                if (type_stack.size() < 2)
                {
                    print_not_enough_arguments_error(op, 2, type_stack.size(), "syscall1");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                type_stack.pop_back();

                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "syscall1");
                    print_op_note(a.op, "syscall number pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_SYSCALL2)
            {
                if (type_stack.size() < 3)
                {
                    print_not_enough_arguments_error(op, 3, type_stack.size(), "syscall2");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();

                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "syscall2");
                    print_op_note(a.op, "syscall number pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_SYSCALL3)
            {
                if (type_stack.size() < 4)
                {
                    print_not_enough_arguments_error(op, 4, type_stack.size(), "syscall3");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();

                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "syscall3");
                    print_op_note(a.op, "syscall number pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_SYSCALL4)
            {
                if (type_stack.size() < 5)
                {
                    print_not_enough_arguments_error(op, 5, type_stack.size(), "syscall4");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();

                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "syscall4");
                    print_op_note(a.op, "syscall number pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_SYSCALL5)
            {
                if (type_stack.size() < 6)
                {
                    print_not_enough_arguments_error(op, 6, type_stack.size(), "syscall5");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();

                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "syscall5");
                    print_op_note(a.op, "syscall number pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
            }
            else if (op.type == OP_SYSCALL6)
            {
                if (type_stack.size() < 7)
                {
                    print_not_enough_arguments_error(op, 7, type_stack.size(), "syscall6");
                    exit(1);
                }
                
                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();
                type_stack.pop_back();

                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "syscall6");
                    print_op_note(a.op, "syscall number pushed here (" + human_readable_type(a.type) + ")");
                    exit(1);
                }
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
                    print_not_enough_arguments_error(op, 1, 0, "do", "while loop", true);
                    exit(1);
                }

                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "do", "while loop", true);
                    print_op_note(a.op, "first value pushed here");
                    exit(1);
                }
                
                StackSnapshot snapshot = stack_snapshots.back(); stack_snapshots.pop_back();
                bool type_stack_equal = compare_type_stacks(snapshot.type_stack, type_stack);

                if (!type_stack_equal)
                {
                    print_op_error(op, "while loops are not allowed to change the types and amount of items on stack in the condition");
                    // TODO: add notes for state of stack
                    exit(1);
                }
                stack_snapshots.push_back(StackSnapshot(type_stack, op.type));
            }
            else if (op.type == OP_IF)
            {
                if (type_stack.size() < 1)
                {
                    print_not_enough_arguments_error(op, 1, 0, "if", "", true);
                    exit(1);
                }

                IluTypeWithOp a = type_stack.back(); type_stack.pop_back();
                if (a.type != DATATYPE_INT)
                {
                    print_invalid_type_error(op, DATATYPE_INT, a.type, "if", "", true);
                    print_op_note(a.op, "first value pushed here");
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
            else if (op.type == OP_END)
            {
                StackSnapshot snapshot = stack_snapshots.back(); stack_snapshots.pop_back();
                bool type_stack_equal = compare_type_stacks(snapshot.type_stack, type_stack);

                if (snapshot.type == OP_IF)
                {
                    if (!type_stack_equal)
                    {
                        print_op_error(op, "if statements are not allowed to change the types of items on stack");
                        // TODO: add notes for state of stack
                        exit(1);
                    }
                }
                else if (snapshot.type == OP_ELSE)
                {
                    if (!type_stack_equal)
                    {
                        print_op_error(op, "both branches of if-else statements must produce the same types on the stack");
                        // TODO: add notes for state of stack
                        exit(1);
                    }
                }
                else if (snapshot.type == OP_DO)
                {
                    if (!type_stack_equal)
                    {
                        print_op_error(op, "while loop are not allowed to change the types of the items on the stack");
                        // TODO: add notes for state of stack
                        exit(1);
                    }
                }
            }
    
            // type checking
            else if (op.type == OP_CAST_PTR)
            {
                type_stack.pop_back();
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
            }
            else if (op.type == OP_CAST_INT)
            {
                type_stack.pop_back();
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
            }

            // lang subset keywords
            else if (op.type == OP_OFFSET || op.type == OP_RESET)
            {
                print_error("OP_OFFSET and OP_RESET are unreachable because they should be handled in the parsing step. This is probably a bug.");
                exit(1);
            }

            // other
            else if (op.type == OP_PUSH_INT)
            {
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
            }
            else if (op.type == OP_PUSH_STR)
            {
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_INT));
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
            }
            else if (op.type == OP_FUNCTION_CALL)
            {
                assert(program.functions.count(op.str_operand));

                Function call_func = program.functions.at(op.str_operand);

                if (type_stack.size() < call_func.arg_stack.size())
                {
                    print_not_enough_arguments_error(op, call_func.arg_stack.size(), type_stack.size(), op.str_operand, "", false, true);
                    exit(1);
                }

                std::vector<IluTypeWithOp> args;
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
                        print_invalid_type_error(op, call_func.arg_stack.at(0).type, args.at(0).type, op.str_operand, "", false, false, true);
                    else
                    {
                        std::vector<DATATYPE> types;
                        for (IluTypeWithOp t : args)
                            types.push_back(t.type);

                        print_invalid_combination_of_types_error(op, types, op.str_operand, "", false, false, true);
                        for (IluTypeWithOp t : args)
                            print_op_note(t.op, "argument pushed here (" + human_readable_type(t.type) + ")");
                    }
                    exit(1);
                }

                for (IluTypeWithOp t : call_func.ret_stack)
                    type_stack.push_back(t);
            }

            else if (op.type == OP_PUSH_GLOBAL_MEM)
            {
                type_stack.push_back(IluTypeWithOp(op, DATATYPE_PTR));
            }
        }

        bool return_vals_equal = compare_type_stacks(type_stack, function.ret_stack);
        if (!return_vals_equal)
        {
            if (function.ret_stack.size() < type_stack.size()) // main cannot have excess data on stack
            {
                print_op_error(function.op, "unhandled data on the stack (expected " + std::to_string(function.ret_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

                std::vector<IluTypeWithOp> excess_stack;
                for (unsigned long int i = type_stack.size() - function.ret_stack.size(); i > 0; i--)
                {
                    excess_stack.push_back(type_stack.back());
                    type_stack.pop_back();
                }
                std::reverse(excess_stack.begin(), excess_stack.end());

                for (IluTypeWithOp t : excess_stack)
                    print_op_note(t.op, "excess data pushed here (" + human_readable_type(t.type) + ")");
            }

            else if (function.ret_stack.size() > type_stack.size())
                print_op_error(function.op, "not enough data on the stack (expected " + std::to_string(function.ret_stack.size()) + " items, got " + std::to_string(type_stack.size()) + ")");

            else if (type_stack.size() > 1)
            {
                std::vector<DATATYPE> types;
                for (IluTypeWithOp t : type_stack)
                    types.push_back(t.type);

                print_invalid_combination_of_types_error(function.op, types, func_name, "", false, true);
                for (IluTypeWithOp t : type_stack)
                    print_op_note(t.op, "argument pushed here (" + human_readable_type(t.type) + ")");
            }

            else
            {
                print_invalid_type_error(function.op, function.ret_stack.at(0).type, type_stack.at(0).type, func_name, "", false, true);
                print_op_note(type_stack.at(0).op, "value pushed here (" + human_readable_type(type_stack.at(0).type) + ")");
            }

            exit(1);
        }
    }
}
