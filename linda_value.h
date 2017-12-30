#ifndef __LINDA_VALUE__
#define __LINDA_VALUE__

#include "linda_base.h"
#include <list>

class LindaValue : public LindaBase {
public:
	LindaValue() {}
	LindaValue(const std::list<LTPair>& l ) : values(l) {}
	std::string toString() const;

	std::list<LTPair> getValues() const
	{ return values; }

	void addValue(LTPair lp)
	{ values.push_back(lp); }

private:
	std::list<LTPair> values;
};

#endif // linda_value
