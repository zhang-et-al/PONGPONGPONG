#include "Ball.hpp"
#include "Icosahedron.hpp"

#include <math.h>
#include <iostream>

std::map<int, Mesh> Ball::presets;

void Ball::setup_mesh()
{
	// Sets the mass of the sphere based on its volume and density
	mass = 4/3 * M_PI * pow(radius, 3) * density;

	int N = round(radius / 15);

	N = N < 2 ? 2 : N;

	if(presets[N].size())
	{
		for(Triangle face : presets[N])
		{
			face.a *= radius/face.a.modulus();
			face.b *= radius/face.b.modulus();
			face.c *= radius/face.c.modulus();
			face.color = color;
			mesh.add(face);
		}

		return;
	}

	Icosahedron ic(0, 0, 0, radius);

	for(Triangle& tri : ic.mesh)
	{
		mesh.add(tri.tessellate(N));
	}
	

	// Project the vertices of the triangles onto the surface of the sphere
	for(Triangle& face : mesh)
	{
		face.a *= radius/face.a.modulus();
		face.b *= radius/face.b.modulus();
		face.c *= radius/face.c.modulus();
		face.color = color;

		/*
		Vector mover = 1 * face.centroid()/radius;

		face.a += mover;
		face.b += mover;
		face.c += mover;
		//*/
	}

	presets[N] = mesh;
}