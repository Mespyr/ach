#ifndef ILU_OPS_H
#define ILU_OPS_H

#include "token.h"

enum OpType 
{
    OP_PUSH,
    
    // ops
    OP_PLUS, 
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_DUMP,
    OP_POP, 
    OP_DUP,  // [a] -> [a, a]
    OP_SWP,  // [a, b] -> [b, a]
    OP_ROT,  // [a, b, c] -> [b, c, a]
    OP_OVER, // [a, b] -> [a, b a]
    OP_MEM,
    OP_LOAD,
    OP_STORE,

    // keywords
    OP_WHILE,
    OP_DO,
    OP_IF,
    OP_ELSE,
    OP_END,
};
    
class Op
{
public:
    Op(OpType type, Token token) :
        type(type), token(token) {}

    Op(OpType type, int push_val, Token token) :
        type(type), push_val(push_val), token(token) {}

    OpType type;
    Token token;
    int push_val;
    int reference_ip;
};

#endif
