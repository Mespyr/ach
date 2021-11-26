#include "../include/ops.h"

Op push(int push_val)
{
    Op op;
    op.push_val = push_val;
    op.type = OP_PUSH;
    return op;
}

Op print()
{
    Op op;
    op.type = OP_PRINT;
    return op;
}

Op plus()
{
    Op op;
    op.type = OP_PLUS;
    return op;
}

Op minus()
{
    Op op;
    op.type = OP_MINUS;
    return op;
}
