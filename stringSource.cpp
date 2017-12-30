#include <stdlib.h>
#include "stringSource.h"

char StringSource::getChar() {
	return curr == value.end() ? EOF : *(curr++) ;
}

char StringSource::peekChar() {
	return curr == value.end() ? EOF : *curr ;
}

void StringSource::setString( const std::string& s) {
	value = s;
	curr = value.begin(); 
}
