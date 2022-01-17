#ifndef ILU_OPS_H
#define ILU_OPS_H

#include "location.h"
#include "token.h"
#include <string>

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
    OP_MEM,
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
    OP_END,
    // other
    OP_PUSH_INT,
    OP_PUSH_STR,
    OP_FUNCTION_CALL,
    OP_NULL
};
    
class Op
{
public:
    Op(OpType type, Token token) :
        type(type),
        loc(token.loc)
    {}

    Op(OpType type, long long push_val, Token token) :
        type(type), 
        loc(token.loc),
        push_int(push_val)
    {}

    Op(OpType type, std::string push_val, Token token) :
        type(type), 
        loc(token.loc),
        push_str(push_val)
    {}

    OpType type;
    Location loc;
    int reference_ip;
    long long push_int;
    std::string push_str;
};

#endif
