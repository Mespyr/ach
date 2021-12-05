#ifndef ILU_OPS_H
#define ILU_OPS_H

enum OpType 
{
    OP_PUSH,
    OP_POP,
    OP_PRINT,
    OP_IF_START,
    OP_IF_END,

    OP_PLUS, 
    OP_MINUS,
    OP_MUL,
    OP_DIV,

    OP_EQUAL
};
    
class Op
{
public:
    Op(OpType type) :
        type(type) {}

    Op(OpType type, int push_val) :
        type(type), push_val(push_val) {}

    void add_reference_to(int ip);

    OpType type;
    int push_val;
    int reference_ip;
};

#endif
