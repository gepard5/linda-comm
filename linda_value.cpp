#include <stdlib.h>
#include <iostream>

#include "linda_value.h"

std::string LindaValue::toString() const
{
	if( values.empty() ) 
		return "()";
	std::string value("( ");
	for( const auto& p : values )
	{
		if( p.first == LindaBase::Type::STRING )
			value.append( "\"");
		value.append(p.second);
		if( p.first == LindaBase::Type::STRING )
			value.append( "\"");
		value.append(", ");
	}
	value.pop_back();
	value.pop_back();
	value.append(" )");
	return value;
}

