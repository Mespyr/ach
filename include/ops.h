#ifndef ILU_OPS_H
#define ILU_OPS_H

enum OpType 
{
    OP_PUSH,
    OP_PRINT,
    OP_PLUS,
    OP_MINUS,
};
    
class Op
{
public:
    Op(OpType type);
    OpType type;
    int push_val;
};

Op push(int push_val);
Op plus();
Op minus();
Op print();

#endif
