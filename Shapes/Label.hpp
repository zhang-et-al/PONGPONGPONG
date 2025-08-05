#include "../Color.hpp"
#include "../Algebra/Vector.hpp"

#include <string>
#include <functional>

struct Label
{
	Vector pos; // relative to the parent Shape

	std::function<std::string(void)> text;

	Color color = Color::white;
};