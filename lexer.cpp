#include "lexer.h"


Lexer::Lexer() {
	whitespace = { ' ', '\t', '\n' };
	token_values = {
		{ OBJECT_START, { "(" } },
		{ OBJECT_END, { ")" } },
		{ VALUE_SEPARATOR, { "," } },
		{ OPERATOR, { "=", "<", ">" } },
		{ STRING_EDGE, { "\"" } },
		{ ZERO, { "0" } },
		{ NUMBER, { "1", "2", "3", "4", "5", "6", "7", "8", "9" } },
		{ MATCHALL_SIGN, { "*" } }
	};
	operator_values = {
		{ LESS, { "<" } },
		{ GREATER, { ">" } },
		{ LESS_THAN, { "<=" } },
		{ GREATER_THAN, { ">=" } },
		{ EQUAL, { "=", "==" } }
	};
}

Token Lexer::getNextToken(Source& source) const {
	std::string curr_token;
	//skip white characters
	while( source.peekChar() != EOF && isWhitespace( source.peekChar() ) ) {
		source.getChar();
	}

	curr_token.push_back( source.getChar() );
	auto type = getTokenType( curr_token );

	if( type == Token::OPERATOR ) {
		if( source.peekChar() == "=" ) {
			curr_token.push_back();
		}
		type = getOperatorType( curr_token );
	}
	else if( type == Token::STRING_EDGE ) {
		while( source.peekChar() != EOF && source.peekChar() != '\"' ) {
			curr_token.push_back( source.getChar() );
		}
		type = Token::STRING;
	}
	else if( type == Token::NUMBER ) {
		auto next_type = getTokenType( std::string( source.peekChar() ) );
		while( next_type == Token::NUMBER || Token::ZERO ) {
			curr_token.push_back( souce.getChar() );
			next_type = getTokenType( std::string( source.peekChar() ) );
		}
	}

	return Token( curr_token, type );
}

Token::TYPE Lexer::getTokenType( const std::string& token ) const
{
	for( const auto& itr : token_values )
	{
		for( const auto& set_itr : itr.second )
		{
			if( set_itr == token ) return itr.first;
		}
	}

	throw UnrecognisedTokenException(token);
}

Token::TYPE Lexer::getOperatorType( const std::string& token ) const
{
	for( const auto& itr : operator_values )
	{
		for( const auto& set_itr : itr.second )
		{
			if( set_itr == token ) return itr.first;
		}
	}

	throw UnrecognisedTokenException(token);
}

