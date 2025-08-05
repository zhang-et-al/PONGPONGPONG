#pragma once

#include "Label.hpp"
#include "../Simulation.hpp"
#include "../Field/Field.hpp"
#include "../Mesh/Mesh.hpp"
#include "../Algebra/Matrix.hpp"
#include "../Collision/Barrier.hpp"

#include <deque>
#include <functional>

class Simulation;

class Shape
{
public:
	enum class Type
	{
		arrow,
		ball,
		icosahedron,
		parallelepiped,
		orthohedron,
		STL_Shape,
		wall,
		visor
	};

	friend class Simulation;
	friend class Barrier;

	Simulation* get_simulation();

	// Translation
	Vector pos;
	Vector vel;
	Vector accel;
	Vector frict_consts;

	// Moment of inertia
	Matrix princ_moment_of_inertia;
	Matrix moment_of_inertia;

	// Trail
	bool keep_trail = false;
	double max_trail_length = 2500;
	double get_trail_length() const;
	std::deque<Vector> trail;

	// Labels
	std::vector<Label> labels;

	// Collision detection
	Vector prev_pos;
	double contact_radius;
	bool collidable;
	void add_barrier(Barrier barrier);

	
	// Although mesh can hold triangles of any color, we'll be sticking for the most part to monochromatic objects
	Color color;

	// ...as well as homogeneus masses
	double density;
	
	double mass;
	
	// Holds the triangles that visually make up this body's appearance
	Mesh mesh;
	
	//Field* field;

	Shape(const Vector& p, const Vector& v, double grav_a = 0, double rad = 3, const Color c = Color::white, double dens = 1, bool collide = true)
	: pos(p), vel(v), accel(0, -grav_a, 0), contact_radius(rad), color(c), density(dens), collidable(collide) {	}
	
	~Shape();
	
	void set_tick_callback(std::function<void(void)> func);
	
	virtual void rotate(Vector axis, bool conserve_angular_momentum = true);

	// May be overridden, but please have the consideration to call its parent 
	virtual void tick_motion(double dt);
	
	virtual Type type() const = 0;


	void set_angular_vel(Vector av);

	inline Vector get_angular_vel() const
	{
		return angular_vel;
	}

	void set_angular_momentum(Vector am);

	inline Vector get_angular_momentum() const
	{
		return angular_momentum;
	}

protected:
	virtual void setup_mesh() = 0;
	
	Simulation* simulation = nullptr;

	std::function<void(void)> on_tick;

	// Rotation
	// Declared protected because use of setters needs to be enforced
	// (Changing the angular velocity must change the angular momentum, and vice versa)
	Vector angular_momentum;
	Vector angle;
	Vector angular_vel;
	Vector angular_accel;

	std::vector<Barrier> barriers;
};
