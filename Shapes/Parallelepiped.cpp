#include "Parallelepiped.hpp"

#include <math.h>
#include <iostream>

void Parallelepiped::setup_mesh()
{
	/*
	std::vector<Vector> points;
	
	points.add(-radius, -radius, -radius);
	points.add(-radius, -radius, radius);
	points.add(-radius, radius, radius);
	points.add(-radius, radius, -radius);
	points.add(radius, -radius, -radius);
	points.add(radius, radius, -radius);
	points.add(-radius, radius, -radius);
	points.add(radius, -radius, radius);
	points.add(radius, radius, radius);
	
	create_triangle_mesh(points, 6.9 * radius);
	*/
	
	Vector v1 = 0, v2 = a, v3 = b, v4 = c, v5 = a + b, v6 = a + c, v7 = b + c, v8 = a + b + c;
	
	/*
	mesh.add(Triangle(v1, v2, v3, color).tessellate());
	mesh.add(Triangle(v5, v2, v3, color).tessellate());

	mesh.add(Triangle(v4, v6, v7, color).tessellate());
	mesh.add(Triangle(v8, v6, v7, color).tessellate());
	
	mesh.add(Triangle(v1, v4, v2, color).tessellate());
	mesh.add(Triangle(v6, v4, v2, color).tessellate());

	mesh.add(Triangle(v3, v5, v7, color).tessellate());
	mesh.add(Triangle(v8, v5, v7, color).tessellate());
	
	mesh.add(Triangle(v3, v1, v7, color).tessellate());
	mesh.add(Triangle(v4, v1, v7, color).tessellate());

	mesh.add(Triangle(v5, v2, v8, color).tessellate());
	mesh.add(Triangle(v6, v2, v8, color).tessellate());
	*/
	//*
	mesh.add(Triangle(v1, v2, v3, color).tessellate());
	mesh.add(Triangle(v5, v2, v3, color).tessellate());

	mesh.add(Triangle(v4, v6, v7, color).tessellate());
	mesh.add(Triangle(v8, v6, v7, color).tessellate());
	
	mesh.add(Triangle(v1, v4, v2, color).tessellate());
	mesh.add(Triangle(v6, v4, v2, color).tessellate());

	mesh.add(Triangle(v3, v5, v7, color).tessellate());
	mesh.add(Triangle(v8, v5, v7, color).tessellate());
	
	mesh.add(Triangle(v3, v1, v7, color).tessellate());
	mesh.add(Triangle(v4, v1, v7, color).tessellate());

	mesh.add(Triangle(v5, v2, v8, color).tessellate());
	mesh.add(Triangle(v6, v2, v8, color).tessellate());
	//*/
	
	// Adjust mesh to center of mass
	mesh -= v8/2;

	mass = abs(a.cross(b).dot(c) * density);
}

void Parallelepiped::rotate(Vector axis, bool conserve_angular_momentum)
{
	Shape::rotate(axis, conserve_angular_momentum);
	a.rotate(axis);
	b.rotate(axis);
	c.rotate(axis);
	base_change = Matrix::Columns(a, b, c).inverse();
}