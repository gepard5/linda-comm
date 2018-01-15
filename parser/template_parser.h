#include <utility>
#include <functional>

#include "parser.h"
#include "lambda_generator.h"
#include "lexer.h"
#include "token.h"
#include "../linda_template.h"

class TemplateParser : public Parser {
public:
	TemplateParser();
	void parseSource( Source& source, Lexer& lexer ) override;
	void showExpectedTokens() override;

	LindaTemplate getLindaTemplate() const;

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

