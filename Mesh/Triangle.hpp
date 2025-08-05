#pragma once

#include "../Algebra/Vector.hpp"
#include "../Color.hpp"

#include <vector>

struct Triangle
{
	Vector a, b, c;
	
	Color color;
	
	Triangle(const Vector& vert1, const Vector& vert2, const Vector& vert3, Color col) 
		: a(vert1), b(vert2), c(vert3), color(col) {}

	/*
	Triangle(Vector&& vert1, Vector&& vert2, Vector&& vert3, Color col)
		: a(std::move(vert1)), b(std::move(vert2)), c(std::move(vert3)), color(col) {}
	*/
	
	Triangle operator+(const Vector& v) const
	{
		return Triangle(a + v, b + v, c + v, color);
	}

	void operator+=(const Vector& v)
	{
		a += v;
		b += v;
		c += v;
	}

	Triangle operator-(const Vector& v) const
	{
		return Triangle(a - v, b - v, c - v, color);
	}

	void operator-=(const Vector& v)
	{
		a -= v;
		b -= v;
		c -= v;
	}

	Triangle operator*(float t) const
	{
		return Triangle(a * t, b * t, c * t, color);
	}

	void operator*=(const Vector& v)
	{
		a *= v;
		b *= v;
		c *= v;
	}

	Triangle operator*(const Vector& v) const
	{
		return Triangle(a * v, b * v, c * v, color);
	}

	void operator*=(float t) {
		a *= t;
		b *= t;
		c *= t;
	}
	
	Vector get_perpendicular_unit() const
	{
		Vector v1 = a - b, v2 = a - c;
		Vector perpendicular = v1.cross(v2);
		return perpendicular/perpendicular.modulus();
	}
	
	bool operator==(const Triangle& tri) const noexcept
	{
		if((a == tri.a && b == tri.b && c == tri.c)
		|| (a == tri.a && c == tri.b && b == tri.c)
		|| (b == tri.a && a == tri.b && c == tri.c)
		|| (b == tri.a && c == tri.b && a == tri.c)
		|| (c == tri.a && a == tri.b && b == tri.c)
		|| (c == tri.a && b == tri.b && a == tri.c))
			return true;
		
		else
			return false;
	}
	
	// For ordering purposes only. Holds little mathematical meaning that I'm aware of.
	bool operator<(const Triangle& tri) const
	{
		if (a != tri.a) return a < tri.a;
		if (b != tri.b) return b < tri.b;
		return c < tri.c;
	}
	bool operator>(const Triangle& tri) const
	{
		if (a != tri.a) return a > tri.a;
		if (b != tri.b) return b > tri.b;
		return c > tri.c;
	}
	
	// Rotates the triangle
	void rotate_azimuth(double angle) {
		a.rotate_azimuth(angle);
		b.rotate_azimuth(angle);
		c.rotate_azimuth(angle);
	}
	void rotate_azimuth(double sin_angle, double cos_angle) {
		a.rotate_azimuth(sin_angle, cos_angle);
		b.rotate_azimuth(sin_angle, cos_angle);
		c.rotate_azimuth(sin_angle, cos_angle);
	}
	void rotate_altitude(double angle) {
		a.rotate_altitude(angle);
		b.rotate_altitude(angle);
		c.rotate_altitude(angle);
	}
	void rotate_altitude(double sin_angle, double cos_angle) {
		a.rotate_altitude(sin_angle, cos_angle);
		b.rotate_altitude(sin_angle, cos_angle);
		c.rotate_altitude(sin_angle, cos_angle);
	}

	void rotate_x(float angle)
	{
		a.rotate_x(angle);
		b.rotate_x(angle);
		c.rotate_x(angle);
	}
	void rotate_y(float angle)
	{
		a.rotate_y(angle);
		b.rotate_y(angle);
		c.rotate_y(angle);
	}
	void rotate_z(float angle)
	{
		a.rotate_z(angle);
		b.rotate_z(angle);
		c.rotate_z(angle);
	}

	void rotate(const Vector& axis)
	{
		a.rotate(axis);
		b.rotate(axis);
		c.rotate(axis);
	}

	float perimeter() const
	{
		return (a-b).modulus() + (b-c).modulus() + (c-a).modulus();
	}

	std::vector<Triangle> tessellate(int N = 0) const;

	Vector centroid() const
	{
		return Vector(a.x + b.x + c.x, a.y + b.y + c.y, a.z + b.z + c.z);
	}
};