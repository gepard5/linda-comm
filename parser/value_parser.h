#include <functional>

#include "../linda_value.h"
#include "parser.h"

class ValueParser : public Parser {
public:
	ValueParser();
	void parseSource( Source& source, Lexer& lexer ) override;
	void showExpectedTokens() override;

	LindaValue getLindaValue() const;

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

