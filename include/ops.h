#ifndef ILU_OPS_H
#define ILU_OPS_H

#include "location.h"
#include "token.h"
#include <string>
#include <vector>

enum OpType 
{
    // debugging
    OP_DUMP,
    // arithmetics
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    // comparisons
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_GREATER_EQ,
    OP_LESS_EQ,
    OP_NOT_EQ,
    OP_NOT,
    OP_AND,
    OP_OR,
    // stack manipulation
    OP_POP,
    OP_DUP,  // [a] -> [a, a]
    OP_SWP,  // [a, b] -> [b, a]
    OP_ROT,  // [a, b, c] -> [b, c, a]
    OP_OVER, // [a, b] -> [a, b a]
    // memory
    OP_READ8,
    OP_WRITE8,
    OP_READ16,
    OP_WRITE16,
    OP_READ32,
    OP_WRITE32,
    OP_READ64,
    OP_WRITE64,
    // argv
    OP_ARGV,
    OP_ARGC,
    // bitwise
    OP_SHIFT_RIGHT,
    OP_SHIFT_LEFT,
    OP_ORB,
    OP_ANDB,
    // syscall ops for number of args to pop from stack
    // ex: syscall3: rax (syscall number), rdi, rsi, rdx
    // ex: syscall5: rax (syscall number), rdi, rsi, rdx, r10, r8
    OP_SYSCALL0,
    OP_SYSCALL1,
    OP_SYSCALL2,
    OP_SYSCALL3,
    OP_SYSCALL4,
    OP_SYSCALL5,
    OP_SYSCALL6,
    // keywords
    OP_WHILE,
    OP_DO,
    OP_IF,
    OP_ELSE,
    OP_DEF,
    OP_CONST,
    OP_MEMORY,
    OP_END,
    OP_INCLUDE,
    // type checking
    OP_CAST_PTR,
    OP_CAST_INT,
    // language subset keywords
    OP_OFFSET,
    OP_RESET,
    // other
    OP_PUSH_INT,
    OP_PUSH_STR,
    OP_PUSH_CSTR,
    OP_FUNCTION_CALL,
    OP_PUSH_GLOBAL_MEM,
    // count | must always be last
    OP_COUNT
};

class Op
{
public:
    Op( Location loc, OpType type) : 
        loc(loc),
        type(type)
    {}

    Op(Location loc, OpType type, unsigned long int int_operand) :
        loc(loc),
        type(type),
        int_operand(int_operand)
    {}

    Op(Location loc, OpType type, std::string str_operand) :
        loc(loc),
        type(type),
        str_operand(str_operand)
    {}

    Location loc;
    OpType type;

    unsigned long int int_operand;
    std::string str_operand;
    bool link_back;
};

bool is_builtin_word(std::string word);

#endif
