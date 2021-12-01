#ifndef ILU_OPS_H
#define ILU_OPS_H

enum OpType 
{
    OP_PUSH,
    OP_PRINT,
    OP_PLUS, 
    OP_MINUS,
    OP_MUL
};
    
class Op
{
public:
    Op(OpType type) :
        type(type) {}

    OpType type;
    int push_val;
};

Op push(int push_val);
Op plus();
Op minus();
Op print();
Op mul();

#endif
