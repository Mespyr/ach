#include "../include/parser.h"

void print_error_if_illegal_word(Token tok, Program program)
{
    if (is_builtin_word(tok.value))
    {
        print_error_at_loc(tok.loc, "built-in word used as const or function definition");
        exit(1);
    }
    else if (is_number(tok.value))
    {
        print_error_at_loc(tok.loc, "numbers are not allowed to be const or function definitions");
        exit(1);
    }
    else if (is_string(tok.value))
    {
        print_error_at_loc(tok.loc, "strings are not allowed to be const or function definitions");
        exit(1);
    }
    else if (program.consts.count(tok.value))
    {
        print_error_at_loc(tok.loc, "redefinition of word '" + tok.value + "' from a const");
        print_note_at_loc(program.consts.at(tok.value).op.loc, "original const defined here");
        exit(1);
    }
    else if (program.functions.count(tok.value))
    {
        print_error_at_loc(tok.loc, "redefinition of word '" + tok.value + "' from a function");
        print_note_at_loc(program.functions.at(tok.value).op.loc, "original function defined here");
        exit(1);
    }
    else if (program.memories.count(tok.value))
    {
        print_error_at_loc(tok.loc, "redefinition of word '" + tok.value + "' from a memory region");
        print_note_at_loc(program.functions.at(tok.value).op.loc, "original memory region defined here");
        exit(1);
    }
}

std::vector<Op> link_ops(std::vector<Op> ops)
{
    static_assert(OP_COUNT == 56, "unhandled op types in link_ops()");

    // track location of newest block parsed
    std::vector<long unsigned int> ip_stack;

    for (long unsigned int ip = 0; ip < ops.size(); ip++)
    {
        Op current_op = ops.at(ip);
        
        if (current_op.type == OP_IF)
            ip_stack.push_back(ip);

        else if (current_op.type == OP_WHILE)
            ip_stack.push_back(ip);

        else if (current_op.type == OP_DO)
        {
            if (ip_stack.size() == 0)
            {
                print_error_at_loc(current_op.loc, "Unexpected 'do' keyword");
                exit(1);
            }
            
            long unsigned int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            if (ops.at(linker_ip).type != OP_WHILE)
            {
                print_error_at_loc(current_op.loc, "Unexpected 'do' keyword");
                exit(1);
            }
            
            // set ip of 'do' keyword to ip of 'while' on stack
            current_op.int_operand = linker_ip;
            ops.at(ip) = current_op;

            // push 'do' ip onto stack
            ip_stack.push_back(ip);
        }

        else if (current_op.type == OP_ELSE)
        {
            if (ip_stack.size() == 0)
            {
                print_error_at_loc(current_op.loc, "Unexpected 'else' keyword");
                exit(1);
            }

            long unsigned int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            // get 'if' op from ops
            Op linker_op = ops.at(linker_ip);

            if (linker_op.type != OP_IF)
            {
                print_error_at_loc(current_op.loc, "Unexpected 'else' keyword");
                exit(1);
            }
            
            // link 'if' op to 'else'
            linker_op.int_operand = ip;
            ops.at(linker_ip) = linker_op;

            // push 'else' ip onto stack
            ip_stack.push_back(ip);
        }

        else if (current_op.type == OP_END)
        {
            if (ip_stack.size() == 0)
            {
                print_error_at_loc(current_op.loc, "Unexpected 'end' keyword");
                exit(1);
            }

            long unsigned int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            // get op at ip on top of stack
            Op linker_op = ops.at(linker_ip);

            if (linker_op.type == OP_IF || linker_op.type == OP_ELSE)
            {
                // unlink 'end' op (it doesn't need to jump anywhere on if statements
                current_op.link_back = false;
                ops.at(ip) = current_op;
                
                // link linker op to 'end' op ip
                linker_op.int_operand = ip;
                ops.at(linker_ip) = linker_op;
            }
            else if (linker_op.type == OP_DO)
            {
                // link 'end' op to 'do' op's ip ('while')
                current_op.int_operand = linker_op.int_operand;
                current_op.link_back = true;
                ops.at(ip) = current_op;

                // link 'do' op to 'end' op's ip
                linker_op.int_operand = ip;
                ops.at(linker_ip) = linker_op;
            }
            else
            {
                print_error_at_loc(current_op.loc, "Unexpected 'end' keyword");
                exit(1);
            }
        }
    }

    return ops;
}

Op convert_token_to_op(Token tok, Program program)
{
    static_assert(OP_COUNT == 56, "unhandled op types in convert_tokens_to_ops()");

    // debugging
    if (tok.value == "dump")
        return Op(OP_DUMP, tok);

    // arithmetics
    else if (tok.value == "+") 
        return Op(OP_PLUS, tok);
    else if (tok.value == "-") 
        return Op(OP_MINUS, tok);
    else if (tok.value == "*")
        return Op(OP_MUL, tok);
    else if (tok.value == "/")
        return Op(OP_DIV, tok);

    // comparisons
    else if (tok.value == "=")
        return Op(OP_EQUAL, tok);
    else if (tok.value == ">")
        return Op(OP_GREATER, tok);
    else if (tok.value == "<")
        return Op(OP_LESS, tok);
    else if (tok.value == ">=")
        return Op(OP_GREATER_EQ, tok);
    else if (tok.value == "<=")
        return Op(OP_LESS_EQ, tok);
    else if (tok.value == "!=")
        return Op(OP_NOT_EQ, tok);
    else if (tok.value == "not")
        return Op(OP_NOT, tok);
    else if (tok.value == "and")
        return Op(OP_AND, tok);
    else if (tok.value == "or")
        return Op(OP_OR, tok);

    // stack manipulation
    else if (tok.value == "pop")
        return Op(OP_POP, tok);
    else if (tok.value == "dup")
        return Op(OP_DUP, tok);
    else if (tok.value == "swp")
        return Op(OP_SWP, tok);
    else if (tok.value == "rot")
        return Op(OP_ROT, tok);
    else if (tok.value == "over")
        return Op(OP_OVER, tok);

    // memory
    else if (tok.value == "read8")
        return Op(OP_READ8, tok);
    else if (tok.value == "write8")
        return Op(OP_WRITE8, tok);
    else if (tok.value == "read16")
        return Op(OP_READ16, tok);
    else if (tok.value == "write16")
        return Op(OP_WRITE16, tok);
    else if (tok.value == "read32")
        return Op(OP_READ32, tok);
    else if (tok.value == "write32")
        return Op(OP_WRITE32, tok);
    else if (tok.value == "read64")
        return Op(OP_READ64, tok);
    else if (tok.value == "write64")
        return Op(OP_WRITE64, tok);

    // argv
    else if (tok.value == "argv")
        return Op(OP_ARGV, tok);
    else if (tok.value == "argc")
        return Op(OP_ARGC, tok);

    // bitwise
    else if (tok.value == "<<")
        return Op(OP_SHIFT_LEFT, tok);
    else if (tok.value == ">>")
        return Op(OP_SHIFT_RIGHT, tok);
    else if (tok.value == "orb")
        return Op(OP_ORB, tok);
    else if (tok.value == "andb")
        return Op(OP_ANDB, tok);

    // syscalls
    else if (tok.value == "syscall1")
        return Op(OP_SYSCALL1, tok);
    else if (tok.value == "syscall2")
        return Op(OP_SYSCALL2, tok);
    else if (tok.value == "syscall3")
        return Op(OP_SYSCALL3, tok);
    else if (tok.value == "syscall4")
        return Op(OP_SYSCALL4, tok);
    else if (tok.value == "syscall5")
        return Op(OP_SYSCALL5, tok);
    else if (tok.value == "syscall6")
        return Op(OP_SYSCALL6, tok);

    // lang subset specific keywords
    else if (tok.value == "offset")
        return Op(OP_OFFSET, tok);
    else if (tok.value == "reset")
        return Op(OP_RESET, tok);

    // keywords
    else if (tok.value == "while")
        return Op(OP_WHILE, tok);
    else if (tok.value == "do")
        return Op(OP_DO, tok);
    else if (tok.value == "if")
        return Op(OP_IF, tok);
    else if (tok.value == "else")
        return Op(OP_ELSE, tok);
    else if (tok.value == "def")
        return Op(OP_DEF, tok);
    else if (tok.value == "const")
        return Op(OP_CONST, tok);
    else if (tok.value == "memory")
        return Op(OP_MEMORY, tok);
    else if (tok.value == "end")
        return Op(OP_END, tok);
    else if (tok.value == "@include")
        return Op(OP_INCLUDE, tok);

    // type checking
    else if (tok.value == "cast(ptr)")
        return Op(OP_CAST_PTR, tok);
    else if (tok.value == "cast(int)")
        return Op(OP_CAST_INT, tok);

    // other
    else if (is_number(tok.value))
        return Op(OP_PUSH_INT, atol(tok.value.c_str()), tok);
    else if (is_string(tok.value))
        return Op(OP_PUSH_STR, add_escapes_to_string(tok.value.substr(1, tok.value.length() - 2)), tok);
    else if (program.consts.count(tok.value))
        return Op(OP_PUSH_INT, program.consts.at(tok.value).value, tok);
    else if (program.functions.count(tok.value))
        return Op(OP_FUNCTION_CALL, tok.value, tok);
    else if (program.memories.count(tok.value))
        return Op(OP_PUSH_GLOBAL_MEM, program.memories.at(tok.value), tok);

    print_error_at_loc(tok.loc, "Unknown keyword '" + tok.value + "'");
    exit(1);
}

Program parse_tokens(std::vector<Token> tokens)
{
    static_assert(OP_COUNT == 56, "unhandled op types in parse_tokens()");

    Program program;

    std::vector<std::string> include_paths;
    long unsigned int iota = 0;
    long unsigned int i = 0;
    int function_addr = 0;

    while (i < tokens.size())
    {
        Op op = convert_token_to_op(tokens.at(i), program);

        if (op.type == OP_DEF)
        {
            i++;
            if (i > tokens.size() - 2)
            {
                print_error_at_loc(op.loc, "unexpected EOF found while parsing function definition");
                exit(1);
            }
            else
            {
                Token func_name_token = tokens.at(i);
                std::string func_name = func_name_token.value;

                // check if function name can be used in code
                print_error_if_illegal_word(func_name_token, program);

                i++;

                std::vector<IluTypeWithOp> arg_stack;
                std::vector<IluTypeWithOp> ret_stack;
                bool pushing_to_arg_stack = true;

                // parse arguments of function
                while (tokens.at(i).value != "in")
                {
                    Token tok = tokens.at(i);

                    if (tok.value == human_readable_type(DATATYPE_INT))
                    {
                        if (pushing_to_arg_stack)
                            arg_stack.push_back(IluTypeWithOp(Op(OP_COUNT, tok), DATATYPE_INT));
                        else
                            ret_stack.push_back(IluTypeWithOp(Op(OP_COUNT, tok), DATATYPE_INT));
                    }
                    else if (tok.value == human_readable_type(DATATYPE_PTR))
                    {
                        if (pushing_to_arg_stack)
                            arg_stack.push_back(IluTypeWithOp(Op(OP_COUNT, tok), DATATYPE_PTR));
                        else
                            ret_stack.push_back(IluTypeWithOp(Op(OP_COUNT, tok), DATATYPE_PTR));
                    }
                    else if (tok.value == "->")
                        pushing_to_arg_stack = false;
                    else
                    {
                        print_error_at_loc(tok.loc, "unknown argument type '" + tok.value + "'");
                        exit(1);
                    }

                    i++;
                    if (i > tokens.size() - 1)
                    {
                        print_error_at_loc(tok.loc, "unexpected EOF found while parsing function definition");
                        exit(1);
                    }
                }

                program.functions.insert({func_name, Function(
                    Op(OP_COUNT, func_name_token),
                    arg_stack,
                    ret_stack,
                    function_addr
                )});
                function_addr++;

                i++;
                if (i >= tokens.size())
                {
                    print_error_at_loc(tokens.back().loc, "unexpected EOF found while parsing function definition");
                    exit(1);
                }

                // get function ops
                std::vector<Op> function_ops;
                bool function_found_end = false;
                int recursion_level = 0;

                while (i < tokens.size())
                {
                    Op f_op = convert_token_to_op(tokens.at(i), program);

                    if (f_op.type == OP_DEF)
                    {
                        print_error_at_loc(f_op.loc, "unexpected 'def' keyword found while parsing. functions cannot be defined inside other functions as there is no scoping.");
                        exit(1);
                    }
                    else if (f_op.type == OP_CONST)
                    {
                        print_error_at_loc(f_op.loc, "unexpected 'const' keyword found while parsing. consts cannot be defined inside functions as there is no scoping.");
                        exit(1);
                    }
                    else if (f_op.type == OP_MEMORY)
                    {
                        print_error_at_loc(f_op.loc, "unexpected 'memory' keyword found while parsing. consts cannot be defined inside functions as there is no scoping.");
                        exit(1);
                    }
                    else if (f_op.type == OP_INCLUDE)
                    {
                        print_error_at_loc(f_op.loc, "unexpected '@include' keyword found while parsing. consts cannot be defined inside functions as there is no scoping.");
                        exit(1);
                    }
                    else if (f_op.type == OP_OFFSET || f_op.type == OP_RESET)
                    {
                        print_error_at_loc(f_op.loc, "'offset' and 'reset' keywords are static, and must be only used inside const definitions");
                        exit(1);
                    }
                    else if (f_op.type == OP_END)
                    {
                        if (recursion_level == 0)
                        {
                            function_found_end = true;
                            break;
                        }
                        else
                            recursion_level--;
                    }
                    else if (f_op.type == OP_IF || f_op.type == OP_WHILE)
                        recursion_level++;

                    function_ops.push_back(f_op);
                    i++;
                }
                if (function_found_end)
                    program.functions.at(func_name).ops = link_ops(function_ops);
                else 
                {
                    print_error_at_loc(tokens.back().loc, "unexpected EOF found while parsing function definition");
                    exit(1);
                }
            }
        }

        else if (op.type == OP_CONST)
        {
            i++;
            if (i > tokens.size() - 2)
            {
                print_error_at_loc(op.loc, "unexpected EOF found while parsing const definition");
                exit(1);
            }
            else
            {
                Token const_name_token = tokens.at(i);
                std::string const_name = const_name_token.value;

                print_error_if_illegal_word(const_name_token, program);
                i++;

                // subset of language in const block supports:
                // pushing integers
                // + and *
                std::vector<long long> stack;

                while (tokens.at(i).value != "end")
                {
                    Op op = convert_token_to_op(tokens.at(i), program);

                    if (op.type == OP_PUSH_INT)
                        stack.push_back(op.int_operand);

                    else if (op.type == OP_PLUS)
                    {
                        if (stack.size() < 2)
                        {
                            print_not_enough_arguments_error(op.loc, 2, stack.size(), "+", "addition");
                            exit(1);
                        }
                        long long a = stack.back(); stack.pop_back();
                        long long b = stack.back(); stack.pop_back();
                        stack.push_back(a + b);
                    }
                    else if (op.type == OP_MUL)
                    {
                        if (stack.size() < 2)
                        {
                            print_not_enough_arguments_error(op.loc, 2, stack.size(), "*", "multiplication");
                            exit(1);
                        }
                        long long a = stack.back(); stack.pop_back();
                        long long b = stack.back(); stack.pop_back();
                        stack.push_back(a * b);
                    }
                    else if (op.type == OP_OFFSET)
                    {
                        if (stack.size() < 1)
                        {
                            print_not_enough_arguments_error(op.loc, 1, 0, "offset");
                            exit(1);
                        }
                        long long a = stack.back(); stack.pop_back();
                        stack.push_back(iota);
                        iota += a;
                    }
                    else if (op.type == OP_RESET)
                    {
                        stack.push_back(iota);
                        iota = 0;
                    }
                    else 
                    {
                        print_error_at_loc(op.loc, "unsuppored keyword in compile-time evaluation of const definition");
                        exit(1);
                    }

                    i++;
                    if (i > tokens.size() - 1)
                    {
                        print_error_at_loc(tokens.at(i - 1).loc, "unexpected EOF found while parsing");
                        exit(1);
                    }
                }

                if (stack.size() > 1)
                {
                    print_error_at_loc(const_name_token.loc, "too many values on stack for const definition (expected 1, got " + std::to_string(stack.size()));
                    exit(1);
                }
                else if (stack.size() < 1)
                {
                    print_error_at_loc(const_name_token.loc, "not enough values on stack for const definition (expected 1, got 0)");
                    exit(1);
                }

                program.consts.insert({const_name, Const(Op(OP_COUNT, const_name_token), stack.back())});
            }
        }

        else if (op.type == OP_MEMORY)
        {
            i++;
            if (i > tokens.size() - 2)
            {
                print_error_at_loc(op.loc, "unexpected EOF found while parsing memory allocation");
                exit(1);
            }
            else
            {
                Token mem_name_token = tokens.at(i);
                std::string mem_name = mem_name_token.value;

                print_error_if_illegal_word(mem_name_token, program);
                i++;

                // subset of language in const block supports:
                // pushing integers
                // + and *
                std::vector<long long> stack;

                while (tokens.at(i).value != "end")
                {
                    Op op = convert_token_to_op(tokens.at(i), program);

                    if (op.type == OP_PUSH_INT)
                        stack.push_back(op.int_operand);

                    else if (op.type == OP_PLUS)
                    {
                        if (stack.size() < 2)
                        {
                            print_not_enough_arguments_error(op.loc, 2, stack.size(), "+", "addition");
                            exit(1);
                        }
                        long long a = stack.back(); stack.pop_back();
                        long long b = stack.back(); stack.pop_back();
                        stack.push_back(a + b);
                    }
                    else if (op.type == OP_MUL)
                    {
                        if (stack.size() < 2)
                        {
                            print_not_enough_arguments_error(op.loc, 2, stack.size(), "*", "multiplication");
                            exit(1);
                        }
                        long long a = stack.back(); stack.pop_back();
                        long long b = stack.back(); stack.pop_back();
                        stack.push_back(a * b);
                    }
                    else
                    {
                        print_error_at_loc(op.loc, "unsuppored keyword in compile-time evaluation of memory definition");
                        exit(1);
                    }

                    i++;
                    if (i > tokens.size() - 1)
                    {
                        print_error_at_loc(tokens.at(i - 1).loc, "unexpected EOF found while parsing");
                        exit(1);
                    }
                }

                if (stack.size() > 1)
                {
                    print_error_at_loc(mem_name_token.loc, "too many values on stack for memory definition (expected 1, got " + std::to_string(stack.size()));
                    exit(1);
                }
                else if (stack.size() < 1)
                {
                    print_error_at_loc(mem_name_token.loc, "not enough values on stack for memory definition (expected 1, got 0)");
                    exit(1);
                }
                long long mem_size = stack.back(); stack.pop_back();
                if (mem_size < 0)
                {
                    exit(1);
                }
                program.memories.insert({mem_name, program.memory_capacity});
                program.memory_capacity += mem_size;
            }
        }

        else if (op.type == OP_INCLUDE)
        {
            if (i++ == tokens.size())
            {
                print_error_at_loc(op.loc, "Unexpected '@include' keyword found while parsing");
                exit(1);
            }

            Token include_file_token = tokens.at(i);
            if (!is_string(include_file_token.value))
            {
                print_error_at_loc(include_file_token.loc, "Was expecting token of type string after @include statement");
                exit(1);
            }

            std::string file_path = include_file_token.value.substr(1,include_file_token.value.length() - 2);
            File file(file_path, MODE_READ);

            if (!file.exists())
            {
                print_error_at_loc(include_file_token.loc, "No such file or directory, '" + file_path + "'");
                exit(1);
            }

            if (std::find(include_paths.begin(), include_paths.end(), file_path) == include_paths.end())
            {
                std::vector<Token> include_file_tokens = tokenize_file(file_path);
                tokens.insert(tokens.begin() + i + 1, include_file_tokens.begin(), include_file_tokens.end());
                include_paths.push_back(file_path);
            }
        }

        else
        {
            print_error_at_loc(op.loc, "unexpected keyword found while parsing");
            exit(1);
        }

        i++;
    }

    return program;
}
