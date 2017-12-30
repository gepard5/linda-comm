#include "linda_communicator.h"
#include "stringSource.h"

LindaValue LindaCommunicator::createValue(const std::string& s)
{
	StringSource source( s );
	value_parser.parseSource( source, lexer );
	return value_parser.getLindaValue();
}

LindaTemplate LindaCommunicator::createTemplate(const std::string& s)
{
	StringSource source( s );
	template_parser.parseSource( source, lexer );
	return template_parser.getLindaTemplate();
}

