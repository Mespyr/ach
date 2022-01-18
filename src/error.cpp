#include "../include/error.h"

void print_error_with_no_location(std::string message)
{
    std::cerr << "ERROR: " << message << std::endl;
}

void print_lexing_error(Location loc, std::string message)
{
    std::cerr << loc.file_location << ":" << loc.line_number << ":" << loc.column_number_start << ": " << message << std::endl;
    std::cerr << loc.line << std::endl;

    for (long unsigned int i = 0; i < loc.column_number_start; i++)
        std::cerr << " ";

    for (long unsigned int i = 0; i < loc.column_number_end - loc.column_number_start; i++)
        std::cerr << "^";

    std::cout << "\n";
}

void print_token_error(Token token, std::string message)
{
    std::cerr << token.loc.file_location << ":" << token.loc.line_number << ":" << token.loc.column_number_start << ": " << message << std::endl;
    std::cerr << token.loc.line << std::endl;

    for (long unsigned int i = 0; i < token.loc.column_number_start; i++)
        std::cerr << " ";

    for (long unsigned int i = 0; i < token.loc.column_number_end - token.loc.column_number_start; i++)
        std::cerr << "^";

    std::cout << "\n";
}

void print_op_error(Op op, std::string message)
{
    std::cerr << op.loc.file_location << ":" << op.loc.line_number << ":" << op.loc.column_number_start << ": " << message << std::endl;
    std::cerr << op.loc.line << std::endl;

    for (long unsigned int i = 0; i < op.loc.column_number_start; i++)
        std::cerr << " ";

    for (long unsigned int i = 0; i < op.loc.column_number_end - op.loc.column_number_start; i++)
        std::cerr << "^";

    std::cout << "\n";
}

void print_op_note(Op op, std::string message)
{
    std::cerr << op.loc.file_location << ":" << op.loc.line_number << ":" << op.loc.column_number_start << ": NOTE: " << message << std::endl;
    std::cerr << op.loc.line << std::endl;

    for (long unsigned int i = 0; i < op.loc.column_number_start; i++)
        std::cerr << " ";

    for (long unsigned int i = 0; i < op.loc.column_number_end - op.loc.column_number_start; i++)
        std::cerr << "^";

    std::cout << "\n";
}

void print_not_enough_arguments_error(Op op, int expected, int actual, std::string name, std::string fullname, bool is_block, bool is_func)
{
    if (fullname == "")
    {
        if (is_block)
            // not enough items on stack for 'name' block (expected 'expected', got 'actual')
            print_op_error(op, "not enough items on stack for '" + name + "' block (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
        else if (is_func)
            // not enough items on stack for function 'name' (expected 'expected', got 'actual')
            print_op_error(op, "not enough items on stack for function '" + name + "' (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
        else
            // not enough items on stack for 'name' operation (expected 'expected', got 'actual')
            print_op_error(op, "not enough items on stack for '" + name + "' operation (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
    }
    else
    {
        if (is_block)
            // not enough items on stack for 'name' (fullname) block (expected 'expected', got 'actual')
            print_op_error(op, "not enough items on stack for '" + name + "' (" + fullname + ") block (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
        else if (is_func)
            // not enough items on stack for function 'name' (fullname) (expected 'expected', got 'actual')
            print_op_error(op, "not enough items on stack for function '" + name + "' (" + fullname + ") (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
        else
            // not enough items on stack for 'name' (fullname) operation (expected 'expected', got 'actual')
            print_op_error(op, "not enough items on stack for '" + name + "' (" + fullname + ") operation (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
    }
}

void print_invalid_combination_of_types_error(Op op, std::vector<DATATYPE> types, std::string name, std::string fullname, bool is_block, bool is_func_ret, bool is_func_args)
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
            print_op_error(op, "invalid combination of types for '" + name + "' condition (" + types_str + ")");
        else if (is_func_ret)
            // invalid combination of types for function 'name' return stack (types_str)
            print_op_error(op, "invalid combination of types for function '" + name + "' return stack (" + types_str + ")");
        else if (is_func_args)
            // invalid combination of types for function 'name' arguments (types_str)
            print_op_error(op, "invalid combination of types for function '" + name + "' arguments (" + types_str + ")");
        else
            // invalid combination of types for 'name' operation (types_str)
            print_op_error(op, "invalid combination of types for '" + name + "' operation (" + types_str + ")");
    }
    else
    {
        if (is_block)
            // invalid combination of types for 'name' (fullname) condition (types_str)
            print_op_error(op, "invalid combination of types for '" + name + "' (" + fullname + ") condition (" + types_str + ")");
        else if (is_func_ret)
            // invalid combination of types for function 'name' (fullname) return stack (types_str)
            print_op_error(op, "invalid combination of types for function '" + name + "' (" + fullname + ") return stack (" + types_str + ")");
        else if (is_func_args)
            // invalid combination of types for function 'name' (fullname) arguments (types_str)
            print_op_error(op, "invalid combination of types for function '" + name + "' (" + fullname + ") arguments (" + types_str + ")");
        else
            // invalid combination of types for 'name' (fullname) operation (types_str)
            print_op_error(op, "invalid combination of types for '" + name + "' (" + fullname + ") operation (" + types_str + ")");
    }
}

void print_invalid_type_error(Op op, DATATYPE expected, DATATYPE actual, std::string name, std::string fullname, bool is_block, bool is_func_ret, bool is_func_args)
{
    if (fullname == "")
    {
        if (is_block)
            // invalid argument type for 'name' condition (expected 'expected', got 'actual')
            print_op_error(op, "invalid type for '" + name + "' condition (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
        else if (is_func_ret)
            // invalid return type for function 'name' (expected 'expected', got 'actual')
            print_op_error(op, "invalid return type for function '" + name + "' (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
        else if (is_func_args)
            // invalid argument type for function 'name' (expected 'expected', got 'actual')
            print_op_error(op, "invalid argument type for function '" + name + "' (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
        else
            // invalid argument type for 'name' operation (expected 'expected', got 'actual')
            print_op_error(op, "invalid type for '" + name + "' operation (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
    }
    else
    {
        if (is_block)
            // invalid argument type for 'name' (fullname) condition (expected 'expected', got 'actual')
            print_op_error(op, "invalid type for '" + name + "' (" + fullname + ") condition (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
        else if (is_func_ret)
            // invalid return type for function 'name' (fullname) (expected 'expected', got 'actual')
            print_op_error(op, "invalid return type for function '" + name + "' (" + fullname + ") (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
        else if (is_func_args)
            // invalid argument type for function 'name' (fullname) (expected 'expected', got 'actual')
            print_op_error(op, "invalid argument type for function '" + name + "' (" + fullname + ") (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
        else
            // invalid argument type for 'name' (fullname) operation (expected 'expected', got 'actual')
            print_op_error(op, "invalid type for '" + name + "' (" + fullname + ") operation (expected " + human_readable_type(expected) + ", got " + human_readable_type(actual) + ")");
    }
}
