#include "Mesh.hpp"

#include <algorithm>

void Mesh::add(const Triangle& tri)
{
	if(prevent_dups)
	{
		if(std::find(faces.begin(), faces.end(), tri) == faces.end()) // NO DUPLICATES!!!!!!!!
		{	
			faces.push_back(tri);
			points.insert(points.end(), {tri.a, tri.b, tri.c});
		}	
	}
	else
	{
		faces.push_back(tri);
		points.insert(points.end(), {tri.a, tri.b, tri.c});
	}
}

void Mesh::add(const Vector& a, const Vector& b, const Vector& c, Color col)
{
	add(Triangle(a, b, c, col));
}

void Mesh::add(const Mesh& mesh)
{
	for(const Triangle& tri : mesh)
		add(tri);
}

void Mesh::add(const std::vector<Triangle>& mesh)
{
	for(const Triangle& tri : mesh)
		add(tri);
}

Mesh::Mesh(const std::vector<Vector>& pts, float max_perimeter)
{
	add_from_points(pts, max_perimeter);
}

void Mesh::add_from_points(const std::vector<Vector>& pts, float max_perimeter, Color col)
{
	for(const Vector& a : pts)
	{
		for(const Vector& b : pts)
		{
			for(const Vector& c : pts)
			{
				if(a == b || c == a || c == b) continue;
				
				Triangle candidate(a, b, c, col);

				if(candidate.perimeter() <= max_perimeter)
				{
					add(candidate);
				}
			}
		}
	}
}
