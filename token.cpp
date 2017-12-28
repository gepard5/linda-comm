#include "token.h"
#include <stdlib.h>

std::map<Token::TYPE, std::string> Token::token_types = {
	{ OBJECT_START, "object_start" },
	{ OBJECT_END, "object_end" },
	{ VALUE_SEPARATOR, "value_separator },
	{ OPERATOR, "operator" },
	{ LESS, "less" },
	{ GREATER, "greater" },
	{ LESS_EQUAL, "less_equal" },
	{ GREATER_EQUAL, "greater_equal" },
	{ EQUAL, "equal" },
	{ MATCHALL_SIGN, "matchall_sign" },
	{ STRING_EDGE, "string_edge", },
	{ STRING, "string" },
	{ END_OF_FILE, "EOF" },
	{ UNRECOGNISED, "unrecognised" },
	{ ZERO, "zero" },
	{ NUMBER, "number" },
	{ ANY, "any" }
};

std::string Token::getTypeString() const
{
	return std::string(token_types[type]);
}

void Token::printInfo() const
{
	std::cout<<"Value: " << value << std::endl;
	std::cout<<"Token type: " << token_types[type] << std::endl;
}
