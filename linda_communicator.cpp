#include <signal.h>

#include "linda_communicator.h"
#include "parser/stringSource.h"

namespace {
	void handler( int x )
	{}
}

LindaCommunicator::LindaCommunicator()
{
	signal(SIGUSR1, handler);
}

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

void LindaCommunicator::output(const std::string& s)
{
	auto lv = createValue( s );
	output( lv );
}

void LindaCommunicator::output(const std::list<LindaBase::LTPair>& lp)
{
	auto lv = LindaValue(lp);
	output( lv );
}

void LindaCommunicator::output(const LindaValue& lv)
{
	file_manager.writeLine( lv.toString() );
}


