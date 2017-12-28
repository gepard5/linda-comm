#include <stdlib.h>

#include "parser.h"

LindaParser::LindaParser() {
	expected_tokens[BEFORE_MESSAGE] = { Token::OBJECT_START };
	expected_tokens[BEFORE_VALUE] = { Token::ZERO, Token::NUMBER, Token::STRING };
	expected_tokens[AFTER_VALUE] = { Token::VALUE_SEPARATOR, Token::OBJECT_END };
	expected_tokens[AFTER_MESSAGE] = { Token::END_OF_FILE };

	semantic_actions[Token::OBJECT_START] = std::bind( &Parser::onObjectStart, this );
	semantic_actions[Token::ZERO] = std::bind( &Parser::onNumber, this );
	semantic_actions[Token::NUMBER] = std::bind( &Parser::onNumber, this );
	semantic_actions[Token::STRING] = std::bind( &Parser::onString, this );
}

void LindaParser::printInfo() const
{
	std::cout<<lv.toString()<<std::endl;
}

bool LindaParser::isTokenTypeExpected(const Token::TYPE& type)
{
	if( type == Token::COMMENT ) return true;
	return expected_tokens[state].count( type ) != 0;
}

void LindaParser::parseSource( Source& source, Lexer& lexer) {
	file = source.getSourceName();
	std::cout<<file<<std::endl;
	state = STATE::BEFORE_MESSAGE;
	token = lexer.getNextToken(source);
	while( token.getType() != Token::END_OF_FILE ) {
		if( !isTokenTypeExpected( token.getType() ) ) {
			throw UnexpectedTokenException( token.getTypeString() );
		}

		semantic_actions[ token.getType() ]();
		token = lexer.getNextToken(source);
	}
}

void LindaParser::onObjectStart() {
	lv = LindaValue();
	state = STATE::BEFORE_VALUE;
}

void LindaParser::onNumber() {
	lv.addValue( { LindaBase::Type::INTEGER, token.getValue() } );
	state = STATE::AFTER_VALUE;
}

void LindaParser::onString() {
	lv.addValue( { LindaBase::Type::STRING, token.getValue() } );
	state = STATE::AFTER_VALUE;
}

void LindaParser::onValueSeparator() {
	state = STATE::BEFORE_VALUE;
}

void LindaParser::onObjectEnd() {
	state = STATE::AFTER_MESSAGE;
}

void LindaParser::printStatus() const
{
	std::cout<<"Parser status"<<std::endl;
}

void LindaParser::showExpectedTokens()
{
	std::cout<<"Expected tokens: "<<std::endl;
	for( auto& t : expected_tokens[state] )
	{
		std::cout<<Token::typeToString(t)<<std::endl;
	}
}
