#include "../Algebra/Base.hpp"
#include "Collision.hpp"

// We're gonna be avoiding polymorphism for this one
class Barrier
{
public:
	enum class Type
	{
		triangle,
		parallelogram,
		segment,
		sphere,
		point
	};

	const Type type;
	const Base base;

	Barrier(Type t, Base b) : type(t), base(b) {}

	/*
	static Barrier Triangle(Base b) : type(Type::triangle), base(b) {}

	static Barrier Parallelogram(Base b) : type(Type::parallelogram), base(b) {}

	static Barrier Segment(Base b) : type(Type::segment), base(b) {}
*/



	static bool is_colliding(const Barrier& b1, const Barrier& b2);

	static Collision is_colliding();

private:

};