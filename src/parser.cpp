#include "../include/parser.h"

std::vector<Op> link_ops(std::vector<Op> ops)
{
    // track location of newest block parsed
    std::vector<int> ip_stack;

    for (int ip = 0; ip < ops.size(); ip++)
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
                print_op_error(current_op, "Unexpected 'do' keyword");
                exit(1);
            }
            
            int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            if (ops.at(linker_ip).type != OP_WHILE)
            {
                print_op_error(current_op, "Unexpected 'do' keyword");
                exit(1);
            }
            
            // set ip of 'do' keyword to ip of 'while' on stack
            current_op.reference_ip = linker_ip;
            ops.at(ip) = current_op;

            // push 'do' ip onto stack
            ip_stack.push_back(ip);
        }

        else if (current_op.type == OP_ELSE)
        {
            if (ip_stack.size() == 0)
            {
                print_op_error(current_op, "Unexpected 'else' keyword");
                exit(1);
            }

            int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            // get 'if' op from ops
            Op linker_op = ops.at(linker_ip);

            if (linker_op.type != OP_IF)
            {
                print_op_error(current_op, "Unexpected 'else' keyword");
                exit(1);
            }
            
            // link 'if' op to 'else'
            linker_op.reference_ip = ip;
            ops.at(linker_ip) = linker_op;

            // push 'else' ip onto stack
            ip_stack.push_back(ip);
        }

        else if (current_op.type == OP_END)
        {
            if (ip_stack.size() == 0)
            {
                print_op_error(current_op, "Unexpected 'end' keyword");
                exit(1);
            }

            int linker_ip = ip_stack.back();
            ip_stack.pop_back();

            // get op at ip on top of stack
            Op linker_op = ops.at(linker_ip);

            if (linker_op.type == OP_IF || linker_op.type == OP_ELSE)
            {
                // unlink 'end' op (it doesn't need to jump anywhere on if statements
                current_op.reference_ip = -1;
                ops.at(ip) = current_op;
                
                // link linker op to 'end' op ip
                linker_op.reference_ip = ip;
                ops.at(linker_ip) = linker_op;
            }
            else if (linker_op.type == OP_DO)
            {
                // link 'end' op to 'do' op's ip ('while')
                current_op.reference_ip = linker_op.reference_ip;
                ops.at(ip) = current_op;

                // link 'do' op to 'end' op's ip
                linker_op.reference_ip = ip;
                ops.at(linker_ip) = linker_op;
            }
            else
            {
                print_op_error(current_op, "Unexpected 'end' keyword");
                exit(1);
            }
        }
    }

    return ops;
}

std::vector<Op> convert_tokens_to_ops(std::vector<Token> tokens, std::map<std::string, std::vector<Token>> basic_program)
{
    std::vector<Op> program;

    for (int i = 0; i < tokens.size(); i++)
    {
        Token tok = tokens.at(i);
        // debugging
        if (tok.value == "dump")
            program.push_back(Op(OP_DUMP, tok));
        
        // arithmetics
        else if (tok.value == "+") 
            program.push_back(Op(OP_PLUS, tok));
        else if (tok.value == "-") 
            program.push_back(Op(OP_MINUS, tok));
        else if (tok.value == "*")
            program.push_back(Op(OP_MUL, tok));
        else if (tok.value == "/")
            program.push_back(Op(OP_DIV, tok));

        // comparisons
        else if (tok.value == "=")
            program.push_back(Op(OP_EQUAL, tok));
        else if (tok.value == ">")
            program.push_back(Op(OP_GREATER, tok));
        else if (tok.value == "<")
            program.push_back(Op(OP_LESS, tok));
        else if (tok.value == ">=")
            program.push_back(Op(OP_GREATER_EQ, tok));
        else if (tok.value == "<=")
            program.push_back(Op(OP_LESS_EQ, tok));
        else if (tok.value == "!=")
            program.push_back(Op(OP_NOT_EQ, tok));
        else if (tok.value == "not")
            program.push_back(Op(OP_NOT, tok));
        else if (tok.value == "and")
            program.push_back(Op(OP_AND, tok));
        else if (tok.value == "or")
            program.push_back(Op(OP_OR, tok));
        
        // stack manipulation
        else if (tok.value == "pop")
            program.push_back(Op(OP_POP, tok));
        else if (tok.value == "dup")
            program.push_back(Op(OP_DUP, tok));
        else if (tok.value == "swp")
            program.push_back(Op(OP_SWP, tok));
        else if (tok.value == "rot")
            program.push_back(Op(OP_ROT, tok));
        else if (tok.value == "over")
            program.push_back(Op(OP_OVER, tok));
        
        // memory
        else if (tok.value == "mem")
            program.push_back(Op(OP_MEM, tok));
        else if (tok.value == "read8")
            program.push_back(Op(OP_READ8, tok));
        else if (tok.value == "write8")
            program.push_back(Op(OP_WRITE8, tok));
        else if (tok.value == "read16")
            program.push_back(Op(OP_READ16, tok));
        else if (tok.value == "write16")
            program.push_back(Op(OP_WRITE16, tok));
        else if (tok.value == "read32")
            program.push_back(Op(OP_READ32, tok));
        else if (tok.value == "write32")
            program.push_back(Op(OP_WRITE32, tok));
        else if (tok.value == "read64")
            program.push_back(Op(OP_READ64, tok));
        else if (tok.value == "write64")
            program.push_back(Op(OP_WRITE64, tok));
       
        // bitwise
        else if (tok.value == "<<")
            program.push_back(Op(OP_SHIFT_LEFT, tok));
        else if (tok.value == ">>")
            program.push_back(Op(OP_SHIFT_RIGHT, tok));
        else if (tok.value == "orb")
            program.push_back(Op(OP_ORB, tok));
        else if (tok.value == "andb")
            program.push_back(Op(OP_ANDB, tok));
        
        // syscalls
        else if (tok.value == "syscall1")
            program.push_back(Op(OP_SYSCALL1, tok));
        else if (tok.value == "syscall2")
            program.push_back(Op(OP_SYSCALL2, tok));
        else if (tok.value == "syscall3")
            program.push_back(Op(OP_SYSCALL3, tok));
        else if (tok.value == "syscall4")
            program.push_back(Op(OP_SYSCALL4, tok));
        else if (tok.value == "syscall5")
            program.push_back(Op(OP_SYSCALL5, tok));
        else if (tok.value == "syscall6")
            program.push_back(Op(OP_SYSCALL6, tok));

        // keywords
        else if (tok.value == "while")
            program.push_back(Op(OP_WHILE, tok));
        else if (tok.value == "do")
            program.push_back(Op(OP_DO, tok));
        else if (tok.value == "if")
            program.push_back(Op(OP_IF, tok));
        else if (tok.value == "else")
            program.push_back(Op(OP_ELSE, tok));
        else if (tok.value == "end")
            program.push_back(Op(OP_END, tok));
        else if (basic_program.count(tok.value))
        {
            std::vector<Token> function_toks = basic_program.at(tok.value);
            // erase function call from token stream
            tokens.erase(tokens.begin() + i);

            tokens.insert(tokens.begin() + i, function_toks.begin(), function_toks.end());
            // dec idx so we can reread token at index
            i--;
        }

        // push
        else if (is_number(tok.value))
            program.push_back(Op(OP_PUSH_INT, atol(tok.value.c_str()), tok));
        else if (is_string(tok.value))
            program.push_back(Op(OP_PUSH_STR, tok.value, tok));

        else
        {
            print_token_error(tok, "Unknown keyword '" + tok.value + "'");
            exit(1);
        }
    }

    return link_ops(program);
}

std::map<std::string, std::vector<Op>> parse_tokens(std::vector<Token> tokens)
{
    std::map<std::string, std::vector<Token>> program;
    std::vector<Token> function_tokens;
    std::vector<std::string> include_paths;
    std::string func_name;
    int i = 0;
    int recursion_level = 0;

    while (i < tokens.size())
    {
        Token tok = tokens.at(i);

        if (tok.value == "def")
        {
            i++;
            // if inside function already
            if (recursion_level > 0 || i >= tokens.size() - 1) 
            {
                print_token_error(tok, "Unexpected 'def' keyword found while parsing");
                exit(1);
            }
            else
            {
                Token func_name_token = tokens.at(i);
                func_name = func_name_token.value;
                // check if function name can be used in code
                if (!is_string(func_name) && !is_number(func_name))
                {
                    if (program.count(func_name))
                    {
                        print_token_error(func_name_token, "Multiple definitions for '" + func_name + "' found");
                        exit(1);
                    }
                    recursion_level++;
                }
                else
                {
                    print_token_error(func_name_token, "Expected word for function name");
                    exit(1);
                }
            }
        }

        else if (tok.value == "@include")
        {
            if (i++ == tokens.size())
            {
                print_token_error(tok, "Unexpected '@include' keyword found while parsing");
                exit(1);
            }

            Token include_file_token = tokens.at(i);
            if (!is_string(include_file_token.value))
            {
                print_token_error(include_file_token, "Was expecting token of type string after @include statement");
                exit(1);
            }

            std::string file_path = include_file_token.value.substr(1,include_file_token.value.length() - 2);
            File file(file_path, MODE_READ);

            if (!file.exists())
            {
                print_token_error(include_file_token, "No such file or directory, '" + file_path + "'");
                exit(1);
            }

            if (std::find(include_paths.begin(), include_paths.end(), file_path) == include_paths.end())
            {
                std::vector<Token> include_file_tokens = tokenize_file(file_path);
                tokens.insert(tokens.end(), include_file_tokens.begin(), include_file_tokens.end());
                include_paths.push_back(file_path);
            }
        }

        else if (tok.value == "end")
        {
            recursion_level--;

            // if outside function block
            if (recursion_level == 0)
            {
                program.insert({func_name, function_tokens});
                function_tokens.clear();
                func_name.clear();
            }
            else function_tokens.push_back(tok);
        }

        // if inside function block
        else if (recursion_level > 0)
        {
            function_tokens.push_back(tok);

            // if code block found, inc recursion_level
            if (tok.value == "if") recursion_level++;
            else if (tok.value == "while") recursion_level++;
        }
        else
        {
            print_token_error(tok, "Unknown keyword '" + tok.value + "' found while parsing");
            exit(1);
        }

        i++;
    }
    
    std::map<std::string, std::vector<Op>> linked_program;

    // loop through all functions in program and convert their tokens into Ops which are linked to each other
    for(auto it = program.begin(); it != program.end(); ++it)
        linked_program.insert({it->first, convert_tokens_to_ops(it->second, program)});

    return linked_program;
}
