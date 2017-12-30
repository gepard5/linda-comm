#include "value_parser.h"

ValueParser::ValueParser() {
	expected_tokens[BEFORE_MESSAGE] = { Token::OBJECT_START };
	expected_tokens[BEFORE_VALUE] = { Token::ZERO, Token::NUMBER, Token::STRING };
	expected_tokens[AFTER_VALUE] = { Token::VALUE_SEPARATOR, Token::OBJECT_END };
	expected_tokens[AFTER_MESSAGE] = { Token::END_OF_FILE };

	semantic_actions[Token::OBJECT_START] = std::bind( &ValueParser::onObjectStart, this );
	semantic_actions[Token::ZERO] = std::bind( &ValueParser::onNumber, this );
	semantic_actions[Token::NUMBER] = std::bind( &ValueParser::onNumber, this );
	semantic_actions[Token::STRING] = std::bind( &ValueParser::onString, this );
	semantic_actions[Token::VALUE_SEPARATOR] = std::bind( &ValueParser::onValueSeparator, this );
	semantic_actions[Token::OBJECT_END] = std::bind( &ValueParser::onObjectEnd, this );
}

bool ValueParser::isTokenTypeExpected(const Token::TYPE& type)
{
	if( type == Token::ANY ) return true;
	return expected_tokens[state].count( type ) != 0;
}

void ValueParser::parseSource( Source& source, Lexer& lexer) {
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

void ValueParser::onObjectStart() {
	lv = LindaValue();
	state = STATE::BEFORE_VALUE;
}

void ValueParser::onNumber() {
	lv.addValue( std::make_pair(LindaBase::Type::INTEGER, token.getValue() ) );
	state = STATE::AFTER_VALUE;
}

void ValueParser::onString() {
	lv.addValue( std::make_pair( LindaBase::Type::STRING, token.getValue() ) );
	state = STATE::AFTER_VALUE;
}

void ValueParser::onValueSeparator() {
	state = STATE::BEFORE_VALUE;
}

void ValueParser::onObjectEnd() {
	state = STATE::AFTER_MESSAGE;
}

void ValueParser::showExpectedTokens()
{
	std::cout<<"Expected tokens: "<<std::endl;
	for( auto& t : expected_tokens[state] )
	{
		std::cout<<Token::typeToString(t)<<std::endl;
	}
}
