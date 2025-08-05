#pragma once

#include "../Algebra/Vector.hpp"
#include "../Algebra/Matrix.hpp"
#include "Triangle.hpp"

#include <functional>
#include <vector>

class Mesh
{
public:
	Mesh() {}

	Mesh(const std::vector<Vector>& pts, float max_perimeter);
	
	void add_from_points(const std::vector<Vector>& pts, float max_perimeter, Color col = Color::white);

	bool prevent_dups = true;

	void add(const Triangle& tri);

	void add(const Vector& a, const Vector& b, const Vector& c, Color col);

	void add(const Mesh& mesh);

	void add(const std::vector<Triangle>& mesh);

	//void erase(std::vector<Triangle>::iterator it);

	//void erase(const Triangle& tri);

	size_t size() const
	{
		return faces.size();
	}

	void clear() noexcept
	{
		faces.clear();
	}

	bool empty() const
	{
		return faces.empty();
	}


	void rotate(Vector axis)
	{
		Matrix rot = Matrix::Rotation(axis);

		for(Triangle& face : faces)
		{
			face.a = rot * face.a;
			face.b = rot * face.b;
			face.c = rot * face.c;
			//face.rotate(axis);
		}
	}

	Mesh operator+(const Vector& v) const
	{
		Mesh m = *this;
		m += v;
		return m;
	}

	void operator+=(const Vector& v)
	{
		for(Triangle& tri : *this)
		{
			tri += v;
		}
	}

	Mesh operator-(const Vector& v) const
	{
		Mesh m = *this;
		m -= v;
		return m;
	}

	void operator-=(const Vector& v)
	{
		for(Triangle& tri : *this)
		{
			tri -= v;
		}
	}

	Mesh operator*(float t) const
	{
		Mesh m = *this;
		m *= t;
		return m;
	}
	void operator*=(float t)
	{
		for(Triangle& tri : *this)
		{
			tri *= t;
		}
	}

	void operator*=(const Vector& v)
	{
		for(Triangle& tri : *this)
		{
			tri *= v;
		}
	}

/*
	Triangle operator*(const Vector& v) const
	{
		return Triangle(a * v, b * v, c * v, color);
	}
*/

	std::vector<Triangle>::iterator begin()
	{
		return faces.begin();
	}

	std::vector<Triangle>::iterator end()
	{
		return faces.end();
	}

	std::vector<Triangle>::const_iterator begin() const
	{
		return faces.cbegin();
	}

	std::vector<Triangle>::const_iterator end() const
	{
		return faces.cend();
	}

private:
	std::vector<Triangle> faces;

	std::vector<Vector> points;
};