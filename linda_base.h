#ifndef __LINDA_BASE__
#define __LINDA_BASE__
#include <utility>
#include <string>

class LindaBase {
public:
	enum class Type {
		INTEGER,
		STRING
	};
	using LTPair = std::pair<Type, std::string>;

	virtual std::string toString() const = 0;
};

#endif // linda_base
