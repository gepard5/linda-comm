#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <set>

#include "token.h"
#include "source.h"


class Lexer {
public:
	Lexer();
	Token getNextToken(Source& source) const;

private:
	Token::TYPE getTokenType( char ) const;
	Token::TYPE getOperatorType( const std::string& ) const;
	using StringSet = std::set<std::string>;
	using TokenValues = std::set< std::pair<Token::TYPE, StringSet> >;

	bool isWhitespace(const char c) const
	{ return whitespace.count(c) == 1; }

	std::set<char> whitespace;
	std::set< std::pair<Token::TYPE, std::set<char>> > token_values;
	TokenValues operator_values;
};

#endif
