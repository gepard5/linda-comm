#include <signal.h>
#include <chrono>
#include <ctime>
#include <thread>

#include "linda_communicator.h"
#include "parser/stringSource.h"
#include "file_exceptions.h"

namespace {
	void handler( int x )
	{}
}

LindaCommunicator::LindaCommunicator(const std::string& s)
{
	file_manager.setFile(s);
	file_manager.clear();
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

LindaValue LindaCommunicator::input(const std::string& s, int timeout)
{
	auto lt = createTemplate( s );
	return input( lt, timeout );
}

LindaValue LindaCommunicator::input(const LindaTemplate& lt, int timeout)
{
	if( timeout < 0 )
		return inputNoTimeout(lt);

	int sleep_time = STARTING_SLEEP;
	LindaValue lv;
	auto start = std::chrono::high_resolution_clock::now();
	int time_passed = -1;
	while ( time_passed < timeout ) {
		lv = read( lt, timeout - time_passed );
		auto current_time = std::chrono::high_resolution_clock::now();
		time_passed = std::chrono::duration<double, std::milli>(current_time - start ).count();
		if( time_passed > timeout )
			time_passed = timeout;

		if( file_manager.deleteLine( lv.toString(), timeout - time_passed ) )
			return lv;

		current_time = std::chrono::high_resolution_clock::now();
		time_passed = std::chrono::duration<double, std::milli>(current_time - start ).count();
	}

	throw LindaNotFoundException();
	return lv;
}

LindaValue LindaCommunicator::inputNoTimeout(const LindaTemplate& lt)
{
	LindaValue lv;
	while ( true ) {
		lv = read( lt, -1);
		if( file_manager.deleteLine( lv.toString() ) )
			break;
	}
	return lv;
}

LindaValue LindaCommunicator::read(const std::string& s, int timeout)
{
	auto lt = createTemplate( s );
	return read( lt, timeout );
}

LindaValue LindaCommunicator::read(const LindaTemplate& lt, int timeout)
{
	if( timeout < 0 )
		return readNoTimeout(lt);

	int sleep_time = STARTING_SLEEP;
	LindaValue lv;
	auto start = std::chrono::high_resolution_clock::now();
	int time_passed = -1;
	while ( time_passed < timeout ) {
		try{
			if( !file_manager.goToNextLine( timeout - time_passed, false ) ) break;

			auto line = file_manager.getCurrentLine();
			lv = createValue( line );
			if( lt.isMatching( lv.getValues() ) ) return lv;
		}
		catch(EndOfFileException& e){
			std::this_thread::sleep_for( std::chrono::milliseconds( sleep_time ) );
			if( sleep_time > MINIMAL_SLEEP )
				sleep_time -= SLEEP_DIFFERENCE;
			file_manager.clear();
		}

		auto current_time = std::chrono::high_resolution_clock::now();
		time_passed = std::chrono::duration<double, std::milli>(current_time - start ).count();
	}

	throw LindaNotFoundException();
	return lv;
}

LindaValue LindaCommunicator::readNoTimeout(const LindaTemplate& lt)
{
	LindaValue lv;
	while ( true ) {
		if( !file_manager.goToNextLine( -1, true ) ) break;

		auto line = file_manager.getCurrentLine();
		lv = createValue( line );
		if( lt.isMatching( lv.getValues() ) )
			break;
	}

	return lv;
}

