#pragma once

#include "Shape.hpp"

#include <iostream>

#include <map>

class Ball : public Shape
{
public:
	float radius;

	Ball(const Vector& p, const Vector& v, double grav_a = 0, double rad = 3, Color c = Color::white, double dens = 1)
	: Shape(p, v, grav_a, rad, c, dens), radius(rad) {
		setup_mesh();
		moment_of_inertia = princ_moment_of_inertia = 2/5 * mass * rad*rad * Matrix::I();
	}

	Type type() const
	{
		return Type::ball;
	}

private:
	void setup_mesh();
	static std::map<int, Mesh> presets;
};
