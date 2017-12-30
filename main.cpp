#include <stdlib.h>

#include "linda_communicator.h"

int main() {
	LindaCommunicator lc;
	std::string s;
	std::string s2;
	std::getline( std::cin, s );
	std::getline( std::cin, s2 );
	auto lt = lc.createTemplate(s2);
	auto lv = lc.createValue(s);
//	std::cout<<lv.toString()<<std::endl;
	if( lt.isMatching( lv.getValues() ) )
		std::cout<<"Working!"<<std::endl;
	return 0;
}
