
#include <list>
#include <string>
#include <functional>

#include "linda_value.h"
#include "linda_template.h"
#include "parser/value_parser.h"
#include "parser/template_parser.h"
#include "parser/lexer.h"
#include "file_manager.h"

class LindaCommunicator {
public:
	LindaCommunicator(const std::string& filename);

	void setFile( const std::string& f )
	{ file_manager.setFile( f ); }

	LindaValue createValue(const std::string&);
	LindaTemplate createTemplate(const std::string&);

	void output(const std::string&);
	void output(const std::list<LindaBase::LTPair>&);
	void output(const LindaValue&);

	LindaValue input(const std::string&, int timeout = -1);
	LindaValue input(const LindaTemplate&, int timeout = -1);

	LindaValue read(const std::string&, int timeout = -1);
	LindaValue read(const LindaTemplate&, int timeout = -1);

	std::vector<std::string> showAll()
	{ return file_manager.getAllLines(); }

private:
	LindaValue readNoTimeout(const LindaTemplate&);
	LindaValue inputNoTimeout(const LindaTemplate&);


	const int MINIMAL_SLEEP{500};
	const int STARTING_SLEEP{1000};
	const int SLEEP_DIFFERENCE{100};
	Lexer lexer;
	ValueParser value_parser;
	TemplateParser template_parser;
	FileManager file_manager;
};
