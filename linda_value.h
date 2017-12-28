#ifndef __LINDA_VALUE__
#define __LINDA_VALUE__

class LindaValue : public LindaBase {
public:
	LindaValue() {}
	LindaValue(const std::list<LTPair>& l ) : values(l) {}
	std::string toString() const override;

	std::list<LTPair> getValues() const
	{ return values; }

	void addValue(LTPair lp)
	{ values.push_back(lp); }

private:
	std::list<LTPair> values;
};

#endif // linda_value
