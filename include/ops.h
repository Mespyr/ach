#ifndef ILU_OPS_H
#define ILU_OPS_H

enum OpType 
{
    OP_PUSH,
    OP_POP,
    OP_PRINT,
    OP_PLUS, 
    OP_MINUS,
    OP_MUL,
    OP_DIV
};
    
class Op
{
public:
    Op(OpType type) :
        type(type) {}

    Op(OpType type, int push_val) :
        type(type), push_val(push_val) {}

    OpType type;
    int push_val;
};

#endif
