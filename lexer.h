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
	Token getNextToken(Source& source);

private:
	void getString(Source&);
	void getOperator(Source&);
	void getSignedNumber(Source&);
	void getNumber(Source&);

	Token::TYPE getTokenType( char ) const;
	Token::TYPE getOperatorType( const std::string& ) const;
	using StringSet = std::set<std::string>;

	bool isWhitespace(const char c) const
	{ return whitespace.count(c) == 1; }

	std::string curr_token;
	Token::TYPE type;
	static const std::set<char> whitespace;
	static const std::set< std::pair<Token::TYPE, std::set<char>> > token_values;
	static const std::set< std::pair<Token::TYPE, StringSet> > operator_values;
	static const char STRING_END;
};

#endif
