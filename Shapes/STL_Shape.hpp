#include "Shape.hpp"

#include <string>

class STL_Shape : public Shape {
private:
	void setup_mesh();
	
public:
	std::string stl_path;
	
	explicit STL_Shape(const std::string& path, const Vector& p, const Vector& v, double grav_a = 0, double rad = 500, bool coll = true)
	 : Shape(p, v, grav_a, rad, Color::white, 0, coll) {
		stl_path = path;
		setup_mesh();
	}

	Type type() const
	{
		return Type::STL_Shape;
	}
};
