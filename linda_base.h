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
};

#endif // linda_base
