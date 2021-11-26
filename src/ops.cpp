#include "../include/ops.h"

Op push(int n)
{
    Op op;
    op.n = n;
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
