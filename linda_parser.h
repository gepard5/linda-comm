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

class LindaParser : public Parser {
public:
	LindaParser();
	void parseSource( Source& source, const Lexer& lexer ) override;
	void printInfo() const override;
	void printStatus() const override;
	void showExpectedTokens() override;

	LindaValue getLindaValue() const
	{ return lv; }

private:
	enum STATE {
		BEFORE_MESSAGE,
		BEFORE_VALUE,
		AFTER_VALUE,
		AFTER_MESSAGE
	};

	void onObjectStart();
	void onNumber();
	void onString();
	void onValueSeparator();
	void onObjectEnd();

	bool isTokenTypeExpected(const Token::TYPE& type);

	LindaValue lv;

	STATE state;
	Token token;
	std::map<STATE, std::set<Token::TYPE> > expected_tokens;
	std::map<Token::TYPE, std::function<void()> > semantic_actions;
};

