#ifndef __PARSER__
#define __PARSER__

#include "lexer.h"
#include "parser_exceptions.h"
#include "token.h"

class Parser {
public:
	virtual void parseSource( Source& source, const Lexer& lexer ) = 0;
	virtual void showExpectedTokens() = 0;

};

#endif //__PARSER__
