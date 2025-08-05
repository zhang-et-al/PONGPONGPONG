#pragma once

#include "Shape.hpp"

class Arrow : public Shape
{
private:
	void setup_mesh();
	
	static Mesh preset;

	Vector direction;

public:

	float radius;

	Vector get_direction() const;

	void set_direction(const Vector& dir);

	Arrow(const Vector& p, const Vector& dir, float rad, Color c = Color::white)
	 : Shape(p, 0, 0, 0, c, 0, false), direction(dir), radius(rad) {
		setup_mesh();
	}

	void rotate(Vector axis)
	{
		for(Triangle& tri : mesh)
		{
			tri.a.rotate(axis);
			tri.b.rotate(axis);
			tri.c.rotate(axis);
		}
	}

	Type type() const
	{
		return Type::arrow;
	}
};
