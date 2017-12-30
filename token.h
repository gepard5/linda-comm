#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

#include <map>

class Token {
public:
	enum TYPE {
		OBJECT_START,							//początek definicji obiektu: "{"
		OBJECT_END,								//koniec definicji obiektu: "}"
		VALUE_SEPARATOR,						//rodziela ciąg wartości : ","
		OPERATOR,								//operator porówania ">", "<", ">=", "<=", "=="
		LESS,
		GREATER,
		LESS_EQUAL,
		GREATER_EQUAL,
		EQUAL,
		MATCHALL_SIGN,							//regex match all "*"
		STRING_EDGE,
		STRING,									//ciągi znaków
		END_OF_FILE,							//koniec pliku
		UNRECOGNISED,
		MINUS,
		ZERO,
		NUMBER,
		ANY
	};


	Token() : type(TYPE::UNRECOGNISED) {}
	Token(const std::string& v, TYPE t ) :
	 type(t), value(v) {}

	void printInfo() const;
	std::string getTypeString() const;

	TYPE getType() const
	{ return type; }

	std::string getValue() const
	{ return value; }

	std::string static typeToString( Token::TYPE t )
	{ return token_types[t]; }
private:
	TYPE type;
	std::string value;
	static std::map<TYPE, std::string> token_types;
};

#endif

