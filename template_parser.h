/*
 * =====================================================================================
 *
 *       Filename:  parser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.04.2017 21:22:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "parser.h"

class TemplateParser : public Parser {
public:
	TemplateParser();
	void parseSource( Source& source, const Lexer& lexer ) override;
	void printInfo() const override;
	void printStatus() const override;
	void showExpectedTokens() override;

	LindaTemplate getLindaValue() const
	{ return lv; }

private:
	enum STATE {
		BEFORE_MESSAGE,
		BEFORE_VALUE,
		AFTER_OPERATOR,
		AFTER_VALUE,
		AFTER_MESSAGE
	};

	void onObjectStart();
	void onEqual();
	void onLess();
	void onLessEqual();
	void onGreater();
	void onGreaterEqual();
	void onAny();
	void onNumber();
	void onString();
	void onValueSeparator();
	void onObjectEnd();

	void addReadyComparator();

	bool isTokenTypeExpected(const Token::TYPE& type);

	LindaTemplate lv;
	LambdaGenerator lambda_generator;

	STATE state;
	Token token;
	std::map<STATE, std::set<Token::TYPE> > expected_tokens;
	std::map<Token::TYPE, std::function<void()> > semantic_actions;
};

