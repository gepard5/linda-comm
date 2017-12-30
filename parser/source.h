#ifndef SOURCE_H
#define SOURCE_H


class Source {
	public:
		virtual char getChar() = 0;
		virtual char peekChar() = 0;
		virtual std::string getSourceName() const = 0;
};

#endif
