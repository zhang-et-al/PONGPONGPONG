#include "Icosahedron.hpp"

constexpr double phi = 1.618033988749;
constexpr double phi2 = phi/2;

void Icosahedron::setup_mesh()
{
	side = contact_radius / phi2;

	std::vector<Vector> points;

	//https://upload.wikimedia.org/wikipedia/commons/9/9c/Icosahedron-golden-rectangles.svg
	//*
	points.emplace_back(phi2 * side, side/2, 0);
	points.emplace_back(phi2 * side, -side/2, 0);
	points.emplace_back(-phi2 * side, -side/2, 0);
	points.emplace_back(-phi2 * side, side/2, 0);
	points.emplace_back(0, phi2 * side, side/2);
	points.emplace_back(0, phi2 * -side, side/2);
	points.emplace_back(0, phi2 * -side, -side/2);
	points.emplace_back(0, phi2 * side, -side/2);
	points.emplace_back(side/2, 0, phi2 * side);
	points.emplace_back(side/2, 0, -phi2 * side);
	points.emplace_back(-side/2, 0, -phi2 * side);
	points.emplace_back(-side/2, 0, phi2 * side);
	//*/
	
	mesh.add_from_points(points, 3.1 * side);
	
	mass = 5*(3+sqrt(5)) * pow(side, 3) / 12;
}
