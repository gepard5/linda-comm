#ifndef __LAMBDA_GENERATOR__
#define __LAMBDA_GENERATOR__

#include <utility>
#include <functional>
#include <map>

#include "../linda_base.h"

class LambdaGenerator {
public:
	enum Type {
		INTEGER,
		STRING,
		NONE_TYPE
	};
	enum Operator {
		LESS,
		GREATER,
		LESS_EQUAL,
		GREATER_EQUAL,
		EQUAL,
		ANY,
		NONE_OPERATOR
	};

	LambdaGenerator();

	void setOperator(Operator o)
	{ curr_operator = o; }

	void setType(Type t)
	{ curr_type = t; }

	void setValue( int v )
	{ curr_value = v; }

	void setString( const std::string& s)
	{ curr_string = s; }

	std::function<bool(std::pair<LindaBase::Type, std::string>)> getComparator();

	bool isReady() const
	{ return curr_type != NONE_TYPE && curr_operator != NONE_OPERATOR; }

	void clearAll();

private:
	using CompareFunction = std::function<bool(std::pair<LindaBase::Type, std::string>)>;

	CompareFunction getNonEqualStringComparator();
	CompareFunction getEqualStringComparator();
	CompareFunction getIntegerComparator();

	std::string curr_string;
	int curr_value;
	Type curr_type;

	std::map<Operator, std::function<bool(int, int)> > integer_functions;
	std::map<Operator, std::function<bool(std::string, std::string)> > string_functions;
	Operator curr_operator;
};



#endif //__lambda_generator__
