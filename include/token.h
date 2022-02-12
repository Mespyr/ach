#ifndef _ACH_TOKEN_H_
#define _ACH_TOKEN_H_

#include "location.h"
#include <string>

enum TokenType
{
	TOKEN_WORD,
	TOKEN_INT,
	TOKEN_STRING,
	TOKEN_C_STRING,
	TOKEN_TYPE_COUNT
};

class Token
{
public: 
	Token(std::string value, TokenType type, Location loc) :
		value(value),
		type(type),
		loc(loc)
	{}

	std::string value;
	TokenType type;
	Location loc;
};

#endif
