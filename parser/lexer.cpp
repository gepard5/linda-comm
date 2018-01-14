#include "lexer.h"
#include "parser_exceptions.h"


const char Lexer::STRING_END = '\"';
const char Lexer::MATCH_ANY = '*';
const std::set< std::pair<Token::TYPE, std::set<char>> > Lexer::token_values = {
	{ Token::OBJECT_START, { '(' } },
	{ Token::OBJECT_END, { ')' } },
	{ Token::VALUE_SEPARATOR, { ',' } },
	{ Token::OPERATOR, { '=', '<', '>' } },
	{ Token::STRING_EDGE, { STRING_END } },
	{ Token::ZERO, { '0' } },
	{ Token::NUMBER, { '1', '2', '3', '4', '5', '6', '7', '8', '9' } },
	{ Token::MINUS, {'-'} },
	{ Token::MATCHALL_SIGN, { MATCH_ANY } },
	{ Token::END_OF_FILE, { EOF, '\0' } }
};
const std::set<char> Lexer::whitespace = { ' ', '\t', '\n' };

const std::set< std::pair<Token::TYPE, Lexer::StringSet> > Lexer::operator_values = {
	{ Token::LESS, { "<" } },
	{ Token::GREATER, { ">" } },
	{ Token::LESS_EQUAL, { "<=" } },
	{ Token::GREATER_EQUAL, { ">=" } },
	{ Token::EQUAL, { "=", "==" } }
};

Token Lexer::getNextToken(Source& source) {
	curr_token.clear();
	//skip white characters
	while( source.peekChar() != EOF && isWhitespace( source.peekChar() ) ) {
		source.getChar();
	}

	curr_token.push_back( source.getChar() );
	type = getTokenType( curr_token[0] );

	if( type == Token::OPERATOR ) {
		getOperator(source);
	}
	else if( type == Token::STRING_EDGE ) {
		getString(source);
	}
	else if( type == Token::NUMBER ) {
		getNumber(source);
	}
	else if( type == Token::MINUS ) {
		getSignedNumber(source);
	}

	if( type == Token::UNRECOGNISED )
		throw UnrecognisedTokenException( curr_token );

	return Token( curr_token, type );
}

void Lexer::getOperator( Source& source )
{
	if( source.peekChar() == '=' ) {
		curr_token.push_back( source.getChar() );
	}
	type = getOperatorType( curr_token );
}

void Lexer::getString( Source& source )
{
	std::string new_string;
	while( source.peekChar() != EOF && source.peekChar() != STRING_END ) {
		new_string.push_back( source.getChar() );
	}
	source.getChar();
	curr_token = new_string;

	type = Token::STRING;
}

void Lexer::getSignedNumber( Source& source )
{
	auto next_type = getTokenType( source.peekChar() );
	if( next_type != Token::NUMBER )
		throw InvalidTokenException( curr_token );

	getNumber(source);
	type = Token::NUMBER;
}

void Lexer::getNumber( Source& source )
{
	auto next_type = getTokenType( source.peekChar() );
	while( next_type == Token::NUMBER || next_type == Token::ZERO ) {
		curr_token.push_back( source.getChar() );
		next_type = getTokenType( source.peekChar() );
	}
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

