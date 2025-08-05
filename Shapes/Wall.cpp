#include "Wall.hpp"

void Wall::setup_mesh()
{
	mesh.add(Triangle(0, a, b, color));
	mesh.add(Triangle(a, b, a+b, color));

	if(centered) mesh -= (a+b)/2;
}