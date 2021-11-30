#include "../include/ops.h"

Op push(int push_val)
{
    Op op(OP_PUSH);
    op.push_val = push_val;
    return op;
}

Op print()
{
    return Op(OP_PRINT);
}

Op plus()
{
    return Op(OP_PLUS);
}

Op minus()
{
    return Op(OP_MINUS);
}
