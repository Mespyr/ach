#include "../include/datatype.h"

std::string human_readable_type(DATATYPE type)
{
    if (type == DATATYPE_INT)
        return "integer";
    return "pointer";
}
