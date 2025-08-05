#include "Shape.hpp"

class Icosahedron : public Shape
{
private:
	void setup_mesh();
	
	float side;

public:

	float get_side() const
	{
		return side;
	}

	explicit Icosahedron(const Vector& p, const Vector& v, double grav_a = 0, double rad = 3, Color c = Color::white, double dens = 1)
	 : Shape(p, v, grav_a, rad, c, dens) {
		setup_mesh();
	}

	Type type() const
	{
		return Type::icosahedron;
	}
};
