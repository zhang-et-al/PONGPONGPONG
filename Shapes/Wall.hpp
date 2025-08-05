#pragma once

#include "Shape.hpp"

class Wall : public Shape {
public:
	Vector a, b;

	bool centered;

	Wall(Vector p, Vector A, Vector B, Color c = Color::white, bool cent = true)
	: Shape(p, 0, 0, (A+B).modulus(), c, 0, true), a(A), b(B), centered(cent) {
		setup_mesh();
	}

	Type type() const
	{
		return Type::wall;
	}

	void rotate(Vector axis)
	{
		Shape::rotate(axis);
		a.rotate(axis);
		b.rotate(axis);
	}
private:
	void setup_mesh();
};