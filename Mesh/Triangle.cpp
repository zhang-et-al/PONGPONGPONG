#include "Triangle.hpp"

std::vector<Triangle> Triangle::tessellate(int N) const
{
	// killswitch for experimental purposes
	//return std::vector<Triangle>{*this};

	if(N < 1)
	{
		N = std::max(2, (int)round(perimeter() / 80));
	}

	float Dx_ac = (c.x - a.x)/N;
	float Dy_ac = (c.y - a.y)/N;
	float Dz_ac = (c.z - a.z)/N;

	float Dx_bc = (c.x - b.x)/N;
	float Dy_bc = (c.y - b.y)/N;
	float Dz_bc = (c.z - b.z)/N;

	std::vector<Vector> points;

	std::vector<Triangle> triangles;

	std::vector<std::vector<Vector>> lattice;

	//points.reserve((N*N + N)/2 + 1);

	//std::cout << "Reserving " << ((N*N + N)/2 + 1) << " elements" << std::endl;
	
	for(int i = 0; i < N; ++i)
	{
		Vector begin(a + Vector(Dx_ac, Dy_ac, Dz_ac) * i);
		Vector end(b + Vector(Dx_bc, Dy_bc, Dz_bc) * i);

		float dx = (end.x - begin.x)/(N-i);
		float dy = (end.y - begin.y)/(N-i);
		float dz = (end.z - begin.z)/(N-i);

		std::vector<Vector> row;

		for(int j = 0; j <= N-i; ++j)
		{
			row.push_back(begin + Vector(dx, dy, dz) * j);
		}

		lattice.push_back(row);
	}

	lattice.push_back(std::vector<Vector>({c}));

	for(int i = 0; i < N; ++i)
	{
		for(int j = 0; j < N-i; ++j)
		{
			triangles.emplace_back(lattice[i][j], lattice[i][j+1], lattice[i+1][j], color);
			
			if(j != N-i-1) triangles.emplace_back(lattice[i][j+1], lattice[i+1][j], lattice[i+1][j+1], color);
		}
	}

	//triangles.emplace_back(lattice[N-1][N-1], lattice[N-1][j+1], lattice[i+1][j], color);


	//std::cout << "Size: " << points.size() << " elements" << std::endl;

	return triangles;
}