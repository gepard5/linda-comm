#include <iostream>

#include "linda_template.h"


bool LindaTemplate::isMatching(std::list<LTPair> lv) const
{
	if( lv.size() != value_checkers.size() )
		return false;

	bool result = true;
	auto value = lv.begin();
	for( auto& checker : value_checkers )
	{
		result &= checker( *value );
		++value;
	}
	return result;
}
