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
		Token::TYPE getTokenType( const std::string& ) const;
		Token::TYPE getOperatorType( const std::string& ) const;

	private:
		typedef std::set<std::string> StringSet;
		using StringSet = std::set<std::string>;
		using TokenValues = std::set< std::pair<Token::TYPE, StringSet> >;

		bool isWhitespace(const char c) const
		{ return whitespace.count(c) == 1; }

		std::set<char> whitespace;
		TokenValues token_values;
		TokenValues operator_values;
};

#endif
