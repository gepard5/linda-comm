
#include <list>
#include <string>
#include <functional>

#include "linda_value.h"
#include "linda_template.h"
#include "value_parser.h"
#include "template_parser.h"
#include "lexer.h"

class LindaCommunicator {
public:
	LindaValue createValue(const std::string&);
	LindaTemplate createTemplate(const std::string&);

	void output(const std::string&);
	void output(const std::list<LindaBase::LTPair>&);
	void output(const LindaBase&);

	LindaValue input(const std::string&, int timeout = -1);
	LindaValue input(const std::list<LindaBase::LTPair>&,
		   	int timeout = -1);
	LindaValue input(const LindaBase&, int timeout = -1);

	LindaValue read(const std::string&, int timeout = -1);
	LindaValue read(const std::list<LindaBase::LTPair>&,
		   	int timeout = -1);
	LindaValue read(const LindaBase&, int timeout = -1);
private:
	Lexer lexer;
	ValueParser value_parser;
	TemplateParser template_parser;
};
