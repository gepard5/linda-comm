#include "lexer.h"
#include "parser_exceptions.h"


Lexer::Lexer() {
	whitespace = { ' ', '\t', '\n' };
	token_values = {
		{ Token::OBJECT_START, { "(" } },
		{ Token::OBJECT_END, { ")" } },
		{ Token::VALUE_SEPARATOR, { "," } },
		{ Token::OPERATOR, { "=", "<", ">" } },
		{ Token::STRING_EDGE, { "\"" } },
		{ Token::ZERO, { "0" } },
		{ Token::NUMBER, { "1", "2", "3", "4", "5", "6", "7", "8", "9" } },
		{ Token::MATCHALL_SIGN, { "*" } }
	};
	operator_values = {
		{ Token::LESS, { "<" } },
		{ Token::GREATER, { ">" } },
		{ Token::LESS_EQUAL, { "<=" } },
		{ Token::GREATER_EQUAL, { ">=" } },
		{ Token::EQUAL, { "=", "==" } }
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
		if( source.peekChar() == '=' ) {
			curr_token.push_back( source.getChar() );
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
		auto next_type = getTokenType( std::string( 1, source.peekChar() ) );
		while( next_type == Token::NUMBER || Token::ZERO ) {
			curr_token.push_back( source.getChar() );
			next_type = getTokenType( std::string( 1, source.peekChar() ) );
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

