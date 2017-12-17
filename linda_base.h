#ifndef __LINDA_BASE__
#define __LINDA_BASE__

class LindaBase {
public:
	enum class Type {
		INTEGER,
		STRING
	};
	using LTPair = std::pair<Type, std::string>;

	virtual std::string toString() const;
};

#endif // linda_base
