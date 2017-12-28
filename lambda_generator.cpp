#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <regex>

#include "lambda_generator.h"
#include "parser_exceptions.h"

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

LambdaGenerator::LambdaGenerator()
{
	integer_functions[LESS] = [](int a, int b) { return a < b; };
	integer_functions[LESS_EQUAL] = [](int a, int b) { return a <= b; };
	integer_functions[GREATER] = [](int a, int b) { return a > b; };
	integer_functions[GREATER_EQUAL] = [](int a, int b) { return a >= b; };
	integer_functions[EQUAL] = [](int a, int b) { return a == b; };
	integer_functions[ANY] = [](int a, int b) { return true; };
	integer_functions[NONE_OPERATOR] = [](int a, int b) { return false; };

	string_functions[LESS] = [](std::string a, std::string b) { return a < b; };
	string_functions[LESS_EQUAL] = [](std::string a, std::string b) { return a <= b; };
	string_functions[GREATER] = [](std::string a, std::string b) { return a > b; };
	string_functions[GREATER_EQUAL] = [](std::string a, std::string b) { return a >= b; };
	string_functions[EQUAL] = [](std::string a, std::string b) { return a == b; };
	string_functions[ANY] = [](std::string a, std::string b) { return true; };
	string_functions[NONE_OPERATOR] = [](std::string a, std::string b) { return false; };
}

void LambdaGenerator::clearAll()
{
	curr_value = 0;
	curr_string = "";
	curr_type = Type::NONE_TYPE;
	curr_operator = Operator::NONE_OPERATOR;
}

std::function<bool(std::pair<LindaBase::Type, std::string>)> LambdaGenerator::getComparator()
{
	if( !isReady() )
		throw UnreadyComparator();

	if( curr_type == Type::STRING ) {
		if( curr_string.empty() ) {
			return [=](std::pair<LindaBase::Type, std::string> p) {
				if( p.first != LindaBase::Type::STRING ) return false;

				return p.second.empty();
			};
		}

		if( curr_operator == Operator::EQUAL ) {
			std::vector<std::string> splitted = split(curr_string, '*');

			return [=](std::pair<LindaBase::Type, std::string> p) {
				if( p.first != LindaBase::Type::STRING ) return false;
				int found = 0;
				for( const auto& s : splitted )
				{
					if( s.empty() ) continue;
					found = p.second.find(s, found);
					if( found == std::string::npos ) {
						return false;
					}
					found += s.size();
				}
				return true;
			};
		}

		if( curr_string.find('*') != std::string::npos )
			throw InvalidTokenException(curr_string);

		return [=](std::pair<LindaBase::Type, std::string> p) {
			if( p.first != LindaBase::Type::STRING ) return false;

			return string_functions[curr_operator](p.second, curr_string);
		};
	}
	else if( curr_type == Type::INTEGER ) {
		return [=](std::pair<LindaBase::Type, std::string> p) {
			if( p.first != LindaBase::Type::INTEGER ) return false;
			int parsed_value;
			try{
				parsed_value = std::stoi( p.second );
			}
			catch(std::invalid_argument& e)
			{ return false; }

			return integer_functions[curr_operator](parsed_value, curr_value);
		};
	}
}
