#include <iostream>
#include "linda_communicator.h"
#include "stringSource.h"

LindaValue LindaCommunicator::createValue(const std::string& s)
{
	StringSource source( s );
	lp.parseSource( source, lexer );
	auto lv = lp.getLindaValue();
	return lv;
}

LindaTemplate LindaCommunicator::createTemplate(const std::string& s)
{
	StringSource source( s );
	lt.parseSource( source, lexer );
	auto lv = lt.getLindaTemplate();
	return lv;
}

