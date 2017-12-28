#include "lexer.h"
#include "parser_exceptions.h"
#include "token.h"

class Parser {
public:
	virtual void parseSource( Source& source, const Lexer& lexer ) = 0;
	virtual void printInfo() const = 0;
	virtual void printStatus() const = 0;
	virtual void showExpectedTokens() = 0;

};

