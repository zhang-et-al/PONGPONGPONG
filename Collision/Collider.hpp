#include "../Shapes/Ball.hpp"
#include "../Shapes/Parallelepiped.hpp"
#include "../Shapes/Wall.hpp"

class Collider
{
public:
	// This function is simply a router for the detect_and_collide functions.
	// It can be given any subclass of Shape and it will choose the right collision detection
	static void try_colliding(Shape* s1, Shape* s2);

	// Detect for collisions
	static void detect_and_collide(Ball* b1, Ball* b2);
	static void detect_and_collide(Ball* b, Parallelepiped* ppd);
	static void detect_and_collide(Ball* b, Wall* w);
	static void detect_and_collide(Parallelepiped* ppd, Wall* w);
	//static void detect_and_collide(Orthohedron* o1, Orthohedron* o2);

	// Perform the collisions
	static void collide(Ball* b1, Ball* b2, double entanglement_depth);
	static void collide(Ball* b, Parallelepiped* ppd, Vector ball_relpos, Vector contact_distance);
	static void collide(Ball* b, Wall* w, Vector contact_distance);
	static void collide(Parallelepiped* ppd, Wall* w, Vector contact_point, Vector contact_distance);
};