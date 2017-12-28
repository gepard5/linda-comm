/*
 * =====================================================================================
 *
 *       Filename:  parser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.04.2017 21:22:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "lexer.h"
#include "parser_exceptions.h"
#include "icinga_object.h"
#include "token.h"

class Parser {
public:
	virtual void parseSource( Source& source, const Lexer& lexer );
	virtual void printInfo() const;
	virtual void printStatus() const;
	virtual void showExpectedTokens();

};

