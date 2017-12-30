#include "lexer.h"
#include "parser_exceptions.h"


Lexer::Lexer() {
	whitespace = { ' ', '\t', '\n' };
	token_values = {
		{ Token::OBJECT_START, { '(' } },
		{ Token::OBJECT_END, { ')' } },
		{ Token::VALUE_SEPARATOR, { ',' } },
		{ Token::OPERATOR, { '=', '<', '>' } },
		{ Token::STRING_EDGE, { '\"' } },
		{ Token::ZERO, { '0' } },
		{ Token::NUMBER, { '1', '2', '3', '4', '5', '6', '7', '8', '9' } },
		{ Token::MATCHALL_SIGN, { '*' } },
		{ Token::END_OF_FILE, { EOF } }
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
	auto type = getTokenType( curr_token[0] );

	if( type == Token::OPERATOR ) {
		if( source.peekChar() == '=' ) {
			curr_token.push_back( source.getChar() );
		}
		type = getOperatorType( curr_token );
	}
	else if( type == Token::STRING_EDGE ) {
		std::string new_string;
		while( source.peekChar() != EOF && source.peekChar() != '\"' ) {
			new_string.push_back( source.getChar() );
		}
		source.getChar();
		curr_token = new_string;
		type = Token::STRING;
	}
	else if( type == Token::NUMBER ) {
		auto next_type = getTokenType( source.peekChar() );
		while( next_type == Token::NUMBER || next_type == Token::ZERO ) {
			curr_token.push_back( source.getChar() );
			next_type = getTokenType( source.peekChar() );
		}
	}
	if( type == Token::UNRECOGNISED )
		throw UnrecognisedTokenException( curr_token );

	return Token( curr_token, type );
}

Token::TYPE Lexer::getTokenType( char token ) const
{
	for( const auto& itr : token_values )
	{
		for( const auto& set_itr : itr.second )
		{
			if( set_itr == token ) return itr.first;
		}
	}

	return Token::UNRECOGNISED;
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

	return Token::UNRECOGNISED;
}

