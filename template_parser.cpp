#include <stdlib.h>

#include "parser.h"

TemplateParser::TemplateParser() {
	expected_tokens[BEFORE_MESSAGE] = { Token::OBJECT_START };
	expected_tokens[BEFORE_VALUE] = { Token::ZERO, Token::NUMBER, Token::STRING, Token::MATCHALL_SIGN,
   		Token::LESS, Token::LESS_EQUAL, Token::GREATER, Token::GREATER_EQUAL, Token::EQUAL	};
	expected_token[AFTER_OPERATOR] = { Token::ZERO, Token::NUMBER, Token::STRING };
	expected_tokens[AFTER_VALUE] = { Token::VALUE_SEPARATOR, Token::OBJECT_END };
	expected_tokens[AFTER_MESSAGE] = { Token::END_OF_FILE };

	semantic_actions[Token::OBJECT_START] = std::bind( &Parser::onObjectStart, this );
	semantic_actions[Token::EQUAL] = std::bind( &Parser::onEqual, this );
	semantic_actions[Token::LESS] = std::bind( &Parser::onLess, this );
	semantic_actions[Token::LESS_EQUAL] = std::bind( &Parser::onLessEqual, this );
	semantic_actions[Token::GREATER] = std::bind( &Parser::onGreater, this );
	semantic_actions[Token::GREATER_EQUAL] = std::bind( &Parser::GreaterEqual, this );
	semantic_actions[Token::MATCHALL_SIGN] = std::bind( &Parser::onAny, this );
	semantic_actions[Token::ZERO] = std::bind( &Parser::onNumber, this );
	semantic_actions[Token::NUMBER] = std::bind( &Parser::onNumber, this );
	semantic_actions[Token::STRING] = std::bind( &Parser::onString, this );
}

void TemplateParser::printInfo() const
{
	std::cout<<lv.toString()<<std::endl;
}

bool TemplateParser::isTokenTypeExpected(const Token::TYPE& type)
{
	if( type == Token::COMMENT ) return true;
	return expected_tokens[state].count( type ) != 0;
}

void TemplateParser::parseSource( Source& source, Lexer& lexer) {
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

void TemplateParser::onObjectStart() {
	lv = LindaTemplate();
	state = STATE::BEFORE_VALUE;
}

void TemplateParser::onEqual() {
	lambda_generator.setOperator(LambdaGenerator::Operator::EQUAL);
	state = STATE::AFTER_OPERATOR;
}

void TemplateParser::onLess() {
	lambda_generator.setOperator(LambdaGenerator::Operator::LESS);
	state = STATE::AFTER_OPERATOR;
}

void TemplateParser::onLessEqual() {
	lambda_generator.setOperator(LambdaGenerator::Operator::LESS_EQUAL);
	state = STATE::AFTER_OPERATOR;
}

void TemplateParser::onGreater() {
	lambda_generator.setOperator(LambdaGenerator::Operator::GREATER);
	state = STATE::AFTER_OPERATOR;
}

void TemplateParser::onGreaterEqual() {
	lambda_generator.setOperator(LambdaGenerator::Operator::GREATER_EQUAL);
	state = STATE::AFTER_OPERATOR;
}

void TemplateParser::onAny() {
	lambda_generator.setOperator(LambdaGenerator::Operator::ANY);
	lambda_generator.setType( LambdaGenerator::Type::INTEGER );
	addReadyComparator();
	state = STATE::AFTER_VALUE;
}

void TemplateParser::onNumber() {
	lambda_generator.setType( LambdaGenerator::Type::INTEGER );
	if( state == STATE::BEFORE_VALUE )
		lambda_generator.setOperator( LambdaGenerator::Operator::EQUAL );

	int parsed_value;
	try{
		parsed_value = std::stoi( token.getValue() );
	}
	catch(std::invalid_argument& e) {
		throw InvalidTokenException( token.getValue() );
	}
	lambda_generator.setValue( parsed_value );

	addReadyComparator();
	state = STATE::AFTER_VALUE;
}

void TemplateParser::onString() {
	lambda_generator.setType( LambdaGenerator::Type::STRING );
	if( state == STATE::BEFORE_VALUE )
		lambda_generator.setOperator( LambdaGenerator::Operator::EQUAL );
	lambda_generator.setString( token.getValue() );

	addReadyComparator();
	state = STATE::AFTER_VALUE;
}

void TemplateParser::onValueSeparator() {
	state = STATE::BEFORE_VALUE;
}

void TemplateParser::onObjectEnd() {
	state = STATE::AFTER_MESSAGE;
}

void TemplateParser::printStatus() const
{
	std::cout<<"Parser status"<<std::endl;
}

void TemplateParser::showExpectedTokens()
{
	std::cout<<"Expected tokens: "<<std::endl;
	for( auto& t : expected_tokens[state] )
	{
		std::cout<<Token::typeToString(t)<<std::endl;
	}
}

void TemplateParser::addReadyComparator()
{
	lv.addValue( lambda_generator.getComparator() );
	lambda_generator.clearAll();
}
