#include "../include/datatype.h"

std::string human_readable_type(DATATYPE type)
{
	static_assert(DATATYPE_COUNT == 2, "unhandled datatypes in human_readable_type()");

	if (type == DATATYPE_INT)
		return "int";
	return "ptr";
}
