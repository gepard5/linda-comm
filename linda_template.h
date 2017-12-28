#ifndef __LINDA_TEMPLATE__
#define __LINDA_TEMPLATE__

class LindaTemplate : public LindaBase {
public:
	LindaTemplate(const std::list<std::function<bool(LTPair)>>& f) :
		value_checkers(f) {}
	std::string toString() const override;
	bool isMatching(std::list<LTPair>) const;
	bool isMatching(const LindaValue& lv) const;

private:
	std::list<std::function<bool(LTPair)>> value_checkers;
};

#endif // linda_template