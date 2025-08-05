#pragma once

#include "Algebra/Vector.hpp"

#include <deque>

class Motion
{
public:
	Vector vel = 0;
	Vector accel = 0;
	Vector frict_consts = 0;

	bool keep_trail = false;
	float max_trail_length = 2500;
	float get_trail_length() const;
	std::deque<Vector> trail;

	Motion(Vector v, Vector a, Vector f)
	: vel(v), accel(a), frict_consts(f) {}
	
	// Updates the velocity based on the acceleration and the friction constants
	void update_vel(float dt);
	
	// Updates a given position after the course of an instant
	void advance_pos(Vector& pos, float dt);

private:
	
};
