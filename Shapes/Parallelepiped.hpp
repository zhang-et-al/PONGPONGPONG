#pragma once

#include "Shape.hpp"

class Parallelepiped : public Shape
{
public:
	
	Parallelepiped(const Vector& p, const Vector& v, const Vector& A, const Vector& B, const Vector& C, double grav_a = 0, Color c = Color::white, double dens = 1)
	 : Shape(p, v, grav_a, (A+B+C).modulus()/2, c, dens), a(A), b(B), c(C) {
		setup_mesh();

		moment_of_inertia = mass/12 * Matrix(
			B.y*B.y+C.z*C.z	,		0	,	0,
			0		,	A.x*A.x+C.z*C.z	,	0,
			0		,		0	,	A.x*A.x+B.y*B.y
		);

		princ_moment_of_inertia = moment_of_inertia;

		base_change = Matrix::Columns(A, B, C).inverse();
	}

	inline static Parallelepiped Orthohedron(const Vector& p, const Vector& v, float A, float B, float C, double grav_a = 0, Color c = Color::white, double dens = 1)
	{
		return Parallelepiped(p, v, Vector(A, 0, 0), Vector(0, B, 0), Vector(0, 0, C), grav_a, c, dens);
	}

	inline static Parallelepiped Cube(const Vector& p, const Vector& v, float edge, double grav_a = 0, Color c = Color::white, double dens = 1)
	{
		return Parallelepiped(p, v, Vector(edge, 0, 0), Vector(0, edge, 0), Vector(0, 0, edge), grav_a, c, dens);
	}

	Type type() const
	{
		return Type::parallelepiped;
	}


	Vector a, b, c;

	void rotate(Vector axis, bool conserve_angular_momentum = true);

	Vector diag() const
	{
		return a+b+c;
	}

	std::array<Vector, 8> get_corners() const
	{
		return std::array<Vector, 8>{ (a+b+c)/2, (-a+b+c)/2, (-a-b+c)/2, (-a-b-c)/2, (a+b-c)/2, (a-b-c)/2, (a-b+c)/2, (-a+b-c)/2 };
	}

	Matrix base_change;

private:
	void setup_mesh();
};
