#ifndef __LINDA_TEMPLATE__
#define __LINDA_TEMPLATE__

#include "linda_base.h"
#include <list>
#include <functional>

class LindaTemplate : public LindaBase {
public:
	LindaTemplate() {}
	LindaTemplate(const std::list<std::function<bool(LTPair)>>& f) :
		value_checkers(f) {}

	bool isMatching(std::list<LTPair>) const;

	void addValue( std::function<bool(LTPair)> f )
	{ value_checkers.push_back(f); }

private:
	std::list<std::function<bool(LTPair)>> value_checkers;
};

#endif // linda_template
