#ifndef __LAMBDA_GENERATOR__
#define __LAMBDA_GENERATOR__

class LambdaGenerator {
public:
	enum Type {
		INTEGER,
		STRING,
		NONE
	};
	enum Operator {
		LESS,
		GREATER,
		LESS_EQUAL,
		GREATER_EQUAL,
		EQUAL,
		ANY,
		STRING,
		NONE
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
	{ return curr_type != NONE && curr_operator != NONE; }

	void clearAll();

private:
	std::string curr_string;
	int curr_value;
	Type curr_type;
	std::map<Operator, std::function<bool(int, int)> > operator_functions;
	std::map<Operator, std::function<bool(std::string, std::string)> > string_functions;
	Operator curr_operator;
};



#endif //__lambda_generator__
