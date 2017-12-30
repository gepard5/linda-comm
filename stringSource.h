#ifndef STRING_SOURCE_H
#define STRING_SOURCE_H

#include <stdlib.h>
#include <string>
#include "source.h"

class StringSource : public Source {
	public:
		StringSource() {}
		StringSource(const std::string& s) :
			value(s) { curr = value.begin(); }

		char getChar();
		char peekChar();
		void setString(const std::string& s);
		std::string getSourceName() const override
		{ return ""; }

	private:
		std::string::iterator curr;
		std::string value;
};

#endif
