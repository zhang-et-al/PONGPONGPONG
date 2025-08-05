#include "../Algebra/Vector.hpp"

// Represents a newly detected collision
struct Collision
{
	Vector point;
	Vector entanglement;

	explicit operator bool() const
      { return false; }
};