#ifndef PARSER_EXCEPTIONS
#define PARSER_EXCEPTIONS

#include <exception>
#include <stdexcept>

class InvalidTokenException : public std::logic_error {
	public:
	InvalidTokenException( const std::string& s ) : 
		std::logic_error( "Invalid Token found: " + s ) {}

};

class UnreadyComparator : public std::logic_error {
	public:
		UnreadyComparator() :
			std::logic_error( "" ) {}
};

class UnrecognisedTokenException : public std::logic_error {
	public:
	UnrecognisedTokenException( const std::string& s ) : 
		std::logic_error( "Unrecognised Token found: " + s ) {}

};

class UnexpectedTokenException : public std::logic_error {
	public:
	UnexpectedTokenException( const std::string& s ) : 
		std::logic_error( "Unexpected Token found: " + s ) {}

};

class UnexpectedTemplateName : public std::logic_error {
	public:
		UnexpectedTemplateName( const std::string& s ) : 
			std::logic_error( "Unexpected Template Name found: " + s ) {}
};

#endif
