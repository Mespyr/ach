#include "../include/parser.h"

std::string add_escapes_to_string(std::string str)
{
    std::string buf;
    std::string ret;
    long unsigned int i = 0;

    // can't get 2 chars if string is 1 or 0 chars
    if (str.length() < 2) return str;

    while (i < str.length())
    {
        buf = str.substr(i, 2);

        if (buf == "\\a")       ret.push_back('\a');
        else if (buf == "\\b")  ret.push_back('\b');
        else if (buf == "\\f")  ret.push_back('\f');
        else if (buf == "\\n")  ret.push_back('\n');
        else if (buf == "\\r")  ret.push_back('\r');
        else if (buf == "\\t")  ret.push_back('\t');
        else if (buf == "\\v")  ret.push_back('\v');
        else if (buf == "\\\\") ret.push_back('\\');
        else if (buf == "\\'")  ret.push_back('\'');
        else if (buf == "\\\"") ret.push_back('\"');
        else if (buf == "\\\?") ret.push_back('\?');
        else if (buf == "\\0")  ret.push_back('\0');
        else
        {
            // if escape sequence not found, shift buffer over by one char to next section
            ret.push_back(buf.at(0));
            i++;
            continue;
        }

        // skip both chars if escape sequence found in buffer
        i+=2;
    }

    return ret;
}

std::vector<Op> link_ops(std::vector<Op> ops)
{
    static_assert(OP_COUNT == 59, "unhandled op types in link_ops()");

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

ConstValueWithContext eval_const_value(Program program, std::vector<Token> tokens, long unsigned int i, long long iota, Location definition_loc)
{
    static_assert(OP_COUNT == 59, "unhandled op types in eval_const_value()");

	// subset of language supports:
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
			print_error_at_loc(op.loc, "unsuppored keyword in compile-time evaluation of expression");
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
		print_error_at_loc(definition_loc, "too many values on stack for definition (expected 1, got " + std::to_string(stack.size()));
		exit(1);
	}
	else if (stack.size() < 1)
	{
		print_error_at_loc(definition_loc, "not enough values on stack for definition (expected 1, got 0)");
		exit(1);
	}
	return ConstValueWithContext { stack.back(), iota, i };
}

Op convert_token_to_op(Token tok, Program program, bool in_function, std::string current_func_name)
{
    static_assert(OP_COUNT == 59, "unhandled op types in convert_tokens_to_ops()");
    static_assert(TOKEN_TYPE_COUNT == 4, "unhandled token types in convert_token_to_op()");

	if (tok.type == TOKEN_WORD)
	{
		// debugging
		if (tok.value == "dump")
			return Op(tok.loc, OP_DUMP);

		// arithmetics
		else if (tok.value == "+") 
			return Op(tok.loc, OP_PLUS);
		else if (tok.value == "-") 
			return Op(tok.loc, OP_MINUS);
		else if (tok.value == "*")
			return Op(tok.loc, OP_MUL);
		else if (tok.value == "/")
			return Op(tok.loc, OP_DIV);

		// comparisons
		else if (tok.value == "=")
			return Op(tok.loc, OP_EQUAL);
		else if (tok.value == ">")
			return Op(tok.loc, OP_GREATER);
		else if (tok.value == "<")
			return Op(tok.loc, OP_LESS);
		else if (tok.value == ">=")
			return Op(tok.loc, OP_GREATER_EQ);
		else if (tok.value == "<=")
			return Op(tok.loc, OP_LESS_EQ);
		else if (tok.value == "!=")
			return Op(tok.loc, OP_NOT_EQ);
		else if (tok.value == "not")
			return Op(tok.loc, OP_NOT);
		else if (tok.value == "and")
			return Op(tok.loc, OP_AND);
		else if (tok.value == "or")
			return Op(tok.loc, OP_OR);

		// stack manipulation
		else if (tok.value == "pop")
			return Op(tok.loc, OP_POP);
		else if (tok.value == "dup")
			return Op(tok.loc, OP_DUP);
		else if (tok.value == "swp")
			return Op(tok.loc, OP_SWP);
		else if (tok.value == "rot")
			return Op(tok.loc, OP_ROT);
		else if (tok.value == "over")
			return Op(tok.loc, OP_OVER);

		// memory
		else if (tok.value == "read8")
			return Op(tok.loc, OP_READ8);
		else if (tok.value == "write8")
			return Op(tok.loc, OP_WRITE8);
		else if (tok.value == "read16")
			return Op(tok.loc, OP_READ16);
		else if (tok.value == "write16")
			return Op(tok.loc, OP_WRITE16);
		else if (tok.value == "read32")
			return Op(tok.loc, OP_READ32);
		else if (tok.value == "write32")
			return Op(tok.loc, OP_WRITE32);
		else if (tok.value == "read64")
			return Op(tok.loc, OP_READ64);
		else if (tok.value == "write64")
			return Op(tok.loc, OP_WRITE64);

		// argv
		else if (tok.value == "argv")
			return Op(tok.loc, OP_ARGV);
		else if (tok.value == "argc")
			return Op(tok.loc, OP_ARGC);

		// bitwise
		else if (tok.value == "<<")
			return Op(tok.loc, OP_SHIFT_LEFT);
		else if (tok.value == ">>")
			return Op(tok.loc, OP_SHIFT_RIGHT);
		else if (tok.value == "orb")
			return Op(tok.loc, OP_ORB);
		else if (tok.value == "andb")
			return Op(tok.loc, OP_ANDB);

		// syscalls
		else if (tok.value == "syscall0")
			return Op(tok.loc, OP_SYSCALL0);
		else if (tok.value == "syscall1")
			return Op(tok.loc, OP_SYSCALL1);
		else if (tok.value == "syscall2")
			return Op(tok.loc, OP_SYSCALL2);
		else if (tok.value == "syscall3")
			return Op(tok.loc, OP_SYSCALL3);
		else if (tok.value == "syscall4")
			return Op(tok.loc, OP_SYSCALL4);
		else if (tok.value == "syscall5")
			return Op(tok.loc, OP_SYSCALL5);
		else if (tok.value == "syscall6")
			return Op(tok.loc, OP_SYSCALL6);

		// lang subset specific keywords
		else if (tok.value == "offset")
			return Op(tok.loc, OP_OFFSET);
		else if (tok.value == "reset")
			return Op(tok.loc, OP_RESET);

		// keywords
		else if (tok.value == "while")
			return Op(tok.loc, OP_WHILE);
		else if (tok.value == "do")
			return Op(tok.loc, OP_DO);
		else if (tok.value == "if")
			return Op(tok.loc, OP_IF);
		else if (tok.value == "else")
			return Op(tok.loc, OP_ELSE);
		else if (tok.value == "def")
			return Op(tok.loc, OP_DEF);
		else if (tok.value == "const")
			return Op(tok.loc, OP_CONST);
		else if (tok.value == "memory")
			return Op(tok.loc, OP_MEMORY);
		else if (tok.value == "end")
			return Op(tok.loc, OP_END);
		else if (tok.value == "@include")
			return Op(tok.loc, OP_INCLUDE);

		// type checking
		else if (tok.value == "cast(ptr)")
			return Op(tok.loc, OP_CAST_PTR);
		else if (tok.value == "cast(int)")
			return Op(tok.loc, OP_CAST_INT);

        // other
		else if (program.consts.count(tok.value))
            return Op(tok.loc, OP_PUSH_INT, program.consts.at(tok.value).value);
		else if (program.functions.count(tok.value))
            return Op(tok.loc, OP_FUNCTION_CALL, tok.value);
		// make local memory have presedence over global memory
		else if (in_function && program.functions.at(current_func_name).memories.count(tok.value))
			return Op(tok.loc, OP_PUSH_LOCAL_MEM, tok.value);
		else if (program.memories.count(tok.value))
			return Op(tok.loc, OP_PUSH_GLOBAL_MEM, tok.value);
    }

    // other
	else if (tok.type == TOKEN_INT)
		return Op(tok.loc, OP_PUSH_INT, atol(tok.value.c_str()));
	else if (tok.type == TOKEN_STRING)
		return Op(tok.loc, OP_PUSH_STR, add_escapes_to_string(tok.value.substr(1, tok.value.length() - 2)));
	else if (tok.type == TOKEN_C_STRING)
		return Op(tok.loc, OP_PUSH_CSTR, add_escapes_to_string(tok.value.substr(1, tok.value.length() - 3)));

    print_error_at_loc(tok.loc, "Unknown keyword '" + tok.value + "'");
    exit(1);
}

Program parse_tokens(std::vector<Token> tokens)
{
    static_assert(OP_COUNT == 59, "unhandled op types in parse_tokens()");

    Program program;

    std::vector<std::string> include_paths;
    long long iota = 0;
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

				if (func_name_token.type == TOKEN_WORD)
				{
					if (is_builtin_word(func_name))
					{
						print_error_at_loc(func_name_token.loc, "const name cannot be a built-in word");
						exit(1);
					}
					else if (program.consts.count(func_name))
					{
						print_error_at_loc(func_name_token.loc, "redefinition of const '" + func_name + "' as a function");
						print_note_at_loc(program.consts.at(func_name).loc, "original const defined here");
						exit(1);
					}
					else if (program.functions.count(func_name))
					{
						print_error_at_loc(func_name_token.loc, "redefinition of function '" + func_name + "'");
						print_note_at_loc(program.functions.at(func_name).loc, "original function defined here");
						exit(1);
					}
					else if (program.memories.count(func_name))
					{
						print_error_at_loc(func_name_token.loc, "redefinition of memory region '" + func_name + "' as a function");
						print_note_at_loc(program.memories.at(func_name).loc, "original memory region defined here");
						exit(1);
					}
				}
				else if (func_name_token.type == TOKEN_INT)
				{
					print_error_at_loc(func_name_token.loc, "function name cannot be an integer");
					exit(1);
				}
				else if (func_name_token.type == TOKEN_STRING)
				{
					print_error_at_loc(func_name_token.loc, "function name cannot be a string");
					exit(1);
				}
				else if (func_name_token.type == TOKEN_C_STRING)
				{
					print_error_at_loc(func_name_token.loc, "function name cannot be a C-string");
					exit(1);
				}

                i++;

                std::vector<TypeAtLoc> arg_stack;
                std::vector<TypeAtLoc> ret_stack;
                bool pushing_to_arg_stack = true;

                // parse arguments of function
                while (tokens.at(i).value != "in")
                {
                    Token tok = tokens.at(i);

                    if (tok.value == human_readable_type(DATATYPE_INT))
                    {
                        if (pushing_to_arg_stack)
                            arg_stack.push_back(TypeAtLoc(tok.loc, DATATYPE_INT));
                        else
                            ret_stack.push_back(TypeAtLoc(tok.loc, DATATYPE_INT));
                    }
                    else if (tok.value == human_readable_type(DATATYPE_PTR))
                    {
                        if (pushing_to_arg_stack)
                            arg_stack.push_back(TypeAtLoc(tok.loc, DATATYPE_PTR));
                        else
                            ret_stack.push_back(TypeAtLoc(tok.loc, DATATYPE_PTR));
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
                    func_name_token.loc,
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
					Op f_op = convert_token_to_op(tokens.at(i), program, true, func_name);

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
						i++;
						if (i > tokens.size() - 2)
						{
							print_error_at_loc(op.loc, "unexpected EOF found while parsing local memory allocation");
							exit(1);
						}
						else
						{
							Token mem_name_token = tokens.at(i);
							std::string mem_name = mem_name_token.value;
							if (mem_name_token.type == TOKEN_WORD)
							{
								if (is_builtin_word(mem_name))
								{
									print_error_at_loc(mem_name_token.loc, "local memory region name cannot be a built-in word");
									exit(1);
								}
								else if (program.consts.count(mem_name))
								{
									print_error_at_loc(mem_name_token.loc, "redefinition of const '" + mem_name + "' as a local memory region");
									print_note_at_loc(program.consts.at(mem_name).loc, "original const defined here");
									exit(1);
								}
								else if (program.functions.count(mem_name))
								{
									print_error_at_loc(mem_name_token.loc, "redefinition of function '" + mem_name + "' as a local memory region");
									print_note_at_loc(program.functions.at(mem_name).loc, "original function defined here");
									exit(1);
								}
								else if (program.functions.at(func_name).memories.count(mem_name))
								{
									print_error_at_loc(mem_name_token.loc, "redefinition of local memory region '" + mem_name + "'");
									print_note_at_loc(program.memories.at(mem_name).loc, "original local memory region defined here");
									exit(1);
								}
							}
							else if (mem_name_token.type == TOKEN_INT)
							{
								print_error_at_loc(mem_name_token.loc, "local memory region name cannot be an integer");
								exit(1);
							}
							else if (mem_name_token.type == TOKEN_STRING)
							{
								print_error_at_loc(mem_name_token.loc, "local memory region name cannot be a string");
								exit(1);
							}
							else if (mem_name_token.type == TOKEN_C_STRING)
							{
								print_error_at_loc(mem_name_token.loc, "local memory region name cannot be a C-string");
								exit(1);
							}
							i++;
							
							ConstValueWithContext m = eval_const_value(program, tokens, i, iota, mem_name_token.loc);
							iota = m.iota;
							i = m.i;
							if (m.value < 0)
							{
								// TODO: once negative numbers are added as parsable values, make sure value is greater than 0
								exit(1);
							}
							program.functions.at(func_name).memories.insert({mem_name, Memory(mem_name_token.loc, program.functions.at(func_name).memory_capacity)});
							program.functions.at(func_name).memory_capacity += m.value;
						}
					}
					else if (f_op.type == OP_INCLUDE)
					{
						print_error_at_loc(f_op.loc, "unexpected '@include' keyword found while parsing. files cannot be included inside functions.");
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
						recursion_level--;
						function_ops.push_back(f_op);
					}	
					else function_ops.push_back(f_op);

					if (f_op.type == OP_IF || f_op.type == OP_WHILE)
						recursion_level++;

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

				if (const_name_token.type == TOKEN_WORD)
				{
					if (is_builtin_word(const_name))
					{
						print_error_at_loc(const_name_token.loc, "const name cannot be a built-in word");
						exit(1);
					}
					else if (program.consts.count(const_name))
					{
						print_error_at_loc(const_name_token.loc, "redefinition of const '" + const_name + "'");
						print_note_at_loc(program.consts.at(const_name).loc, "original const defined here");
						exit(1);
					}
					else if (program.functions.count(const_name))
					{
						print_error_at_loc(const_name_token.loc, "redefinition of function '" + const_name + "' as a const");
						print_note_at_loc(program.functions.at(const_name).loc, "original function defined here");
						exit(1);
					}
					else if (program.memories.count(const_name))
					{
						print_error_at_loc(const_name_token.loc, "redefinition of memory region '" + const_name + "' as a const");
						print_note_at_loc(program.memories.at(const_name).loc, "original memory region defined here");
						exit(1);
					}
				}
				else if (const_name_token.type == TOKEN_INT)
				{
					print_error_at_loc(const_name_token.loc, "const name cannot be an integer");
					exit(1);
				}
				else if (const_name_token.type == TOKEN_STRING)
				{
					print_error_at_loc(const_name_token.loc, "const name cannot be a string");
					exit(1);
				}
				else if (const_name_token.type == TOKEN_C_STRING)
				{
					print_error_at_loc(const_name_token.loc, "const name cannot be a C-string");
					exit(1);
				}
                i++;

				ConstValueWithContext c = eval_const_value(program, tokens, i, iota, const_name_token.loc);
				iota = c.iota;
				i = c.i;
                program.consts.insert({const_name, Const(const_name_token.loc, c.value)});
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

				if (mem_name_token.type == TOKEN_WORD)
				{
					if (is_builtin_word(mem_name))
					{
						print_error_at_loc(mem_name_token.loc, "memory region name cannot be a built-in word");
						exit(1);
					}
					else if (program.consts.count(mem_name))
					{
						print_error_at_loc(mem_name_token.loc, "redefinition of const '" + mem_name + "' as a memory region");
						print_note_at_loc(program.consts.at(mem_name).loc, "original const defined here");
						exit(1);
					}
					else if (program.functions.count(mem_name))
					{
						print_error_at_loc(mem_name_token.loc, "redefinition of function '" + mem_name + "' as a memory region");
						print_note_at_loc(program.functions.at(mem_name).loc, "original function defined here");
						exit(1);
					}
					else if (program.memories.count(mem_name))
					{
						print_error_at_loc(mem_name_token.loc, "redefinition of memory region '" + mem_name + "'");
						print_note_at_loc(program.memories.at(mem_name).loc, "original memory region defined here");
						exit(1);
					}
				}
				else if (mem_name_token.type == TOKEN_INT)
				{
					print_error_at_loc(mem_name_token.loc, "memory region name cannot be an integer");
					exit(1);
				}
				else if (mem_name_token.type == TOKEN_STRING)
				{
					print_error_at_loc(mem_name_token.loc, "memory region name cannot be a string");
					exit(1);
				}
				else if (mem_name_token.type == TOKEN_C_STRING)
				{
					print_error_at_loc(mem_name_token.loc, "memory region name cannot be a C-string");
					exit(1);
				}
                i++;
				
                ConstValueWithContext m = eval_const_value(program, tokens, i, iota, mem_name_token.loc);
				iota = m.iota;
				i = m.i;
                if (m.value < 0)
                {
					// TODO: once negative numbers are added as parsable values, make sure value is greater than 0
                    exit(1);
                }
                program.memories.insert({mem_name, Memory(mem_name_token.loc, program.memory_capacity)});
                program.memory_capacity += m.value;
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
            if (include_file_token.type != TOKEN_STRING)
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
