#include "STL_Shape.hpp"

#include <SDL2/SDL_image.h>
#include <algorithm>
#include <fstream>
#include <iostream>

void STL_Shape::setup_mesh()
{
	
	mass = 4/3 * M_PI * pow(contact_radius, 3) * density; // spherical cows.

	std::ifstream file(stl_path, std::ios::binary);
	
	if (file.fail())
	{
		throw std::runtime_error("Failed to open file");
	}

	// Skip header
	file.seekg(80);

	// Read number of triangles
	uint32_t num_triangles;
	file.read(reinterpret_cast<char*>(&num_triangles), sizeof(num_triangles));

	float max_distance = 0;


	std::cout << "Reading triangles..." << std::endl;

	mesh.prevent_dups = false;
	// Read triangles
	for (uint32_t i = 0; i < num_triangles; ++i)
	{
		//Vector a, b, c;
		float a1, a2, a3, b1, b2, b3, c1, c2, c3;

		//file.read(reinterpret_cast<char*>(&triangle.normal), sizeof(Vector3));
		file.ignore(12); // Skip normal vector
		file.read(reinterpret_cast<char*>(&a1), 4);
		file.read(reinterpret_cast<char*>(&a2), 4);
		file.read(reinterpret_cast<char*>(&a3), 4);
		file.read(reinterpret_cast<char*>(&b1), 4);
		file.read(reinterpret_cast<char*>(&b2), 4);
		file.read(reinterpret_cast<char*>(&b3), 4);
		file.read(reinterpret_cast<char*>(&c1), 4);
		file.read(reinterpret_cast<char*>(&c2), 4);
		file.read(reinterpret_cast<char*>(&c3), 4);

		Vector a(a1, a2, a3);
		Vector b(b1, b2, b3);
		Vector c(c1, c2, c3);


		file.ignore(2); // Skip attribute byte count

		mesh.add(a, b, c, Color::white); //Color(50 + (char)200*((float) num_triangles/i), 50 + (char)200*((float) num_triangles/i), 50 + (char)200*((float) num_triangles/i)));

		if(a.modulus() > max_distance) max_distance = a.modulus();
		if(b.modulus() > max_distance) max_distance = b.modulus();
		if(c.modulus() > max_distance) max_distance = c.modulus();
	}


	Vector shift_down(0, max_distance/2, 0);

	for(Triangle& face : mesh) {
		face -= shift_down;
		face *= contact_radius/max_distance;
	}

	std::cout << "Done, read " << mesh.size() << " triangles." << std::endl;
}