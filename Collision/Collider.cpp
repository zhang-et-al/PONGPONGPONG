#include "Collider.hpp"
#include <cmath>

void Collider::try_colliding(Shape* s1, Shape* s2)
{
	if(!s1->collidable || !s2->collidable) return;

	if((s1->pos - s2->pos).modulus() > (s1->contact_radius + s2->contact_radius)) return;

	if(s1 == s2) return;

	/*
	for(const Barrier& b1 : s1.barriers)
	{
		for(const Barrier& b2 : s2.barriers)
		{
			if(Barrier::is_colliding(s1, s2))
		}
	}*/
	switch(s1->type())
	{
		case Shape::Type::ball:
			switch(s2->type())
			{
				case Shape::Type::ball:
					detect_and_collide((Ball*) s1, (Ball*) s2);
					break;

				case Shape::Type::parallelepiped:
					detect_and_collide((Ball*) s1, (Parallelepiped*) s2);
					break;

				case Shape::Type::wall:
					detect_and_collide((Ball*) s1, (Wall*) s2);
					break;
			}
			break;

		case Shape::Type::parallelepiped:
			switch(s2->type())
			{
				case Shape::Type::ball:
					detect_and_collide((Ball*) s2, (Parallelepiped*) s1);
					break;

				case Shape::Type::wall:
					detect_and_collide((Parallelepiped*) s1, (Wall*) s2);
					break;
			}
			break;

		case Shape::Type::wall:
			switch(s2->type())
			{
				case Shape::Type::ball:
					detect_and_collide((Ball*) s2, (Wall*) s1);
					break;

				case Shape::Type::parallelepiped:
					detect_and_collide((Parallelepiped*) s2, (Wall*) s1);
					break;
				/*
				case Type::orthohedron:
					detect_and_collide((Orthohedron*) this, (Orthohedron*) s);
					break;
				*/
			}
			break;
	}
}


void Collider::collide(Ball* b1, Ball* b2, double entanglement_depth)
{
	Vector distance = b1->pos - b2->pos;
	
	// Disentangle
	Vector excess = entanglement_depth * (b1->pos - b2->pos).normalise()/2;
	b1->pos += excess;
	b2->pos -= excess;
	
	// And collide
	// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
	// https://wikimedia.org/api/rest_v1/media/math/render/svg/14d5feb68844edae9e31c9cb4a2197ee922e409c

	Vector v = -distance * (b2->vel - b1->vel).dot(b2->pos - b1->pos) / (b2->pos - b1->pos).modulus2();

	b1->vel = b1->vel + (2 * b2->mass)/(b1->mass + b2->mass) * v;
	b2->vel = b2->vel - (2 * b1->mass)/(b1->mass + b2->mass) * v;

	b1->vel *= 0.9;
}

void Collider::collide(Ball* b, Parallelepiped* ppd, Vector ball_relpos, Vector contact_distance)
{
	Vector n = contact_distance.normalise();
	Vector rxn = ball_relpos.cross(n);

	Matrix I_1 = ppd->moment_of_inertia.inverse();

	double impulse = 2 * (rxn.dot(ppd->get_angular_vel()) - n.dot(b->vel - ppd->vel)) / ((ppd->mass+b->mass)/(ppd->mass*b->mass) + (I_1*rxn).dot(rxn));
	ppd->set_angular_vel(ppd->get_angular_vel() - impulse * I_1 * rxn);

	ppd->vel = -impulse/ppd->mass * n + ppd->vel;
	b->vel = impulse/b->mass * n + b->vel;

	b->pos += n * (b->radius-contact_distance.modulus());
}

void Collider::collide(Ball* b, Wall* w, Vector contact_distance)
{
	Vector n = contact_distance.normalise();
	b->vel = 0.99 * (b->vel - 2 * (n.dot(b->vel)) * n);
	b->pos += n * (b->radius-contact_distance.modulus());
}

void Collider::collide(Parallelepiped* ppd, Wall* w, Vector contact_point, Vector contact_distance)
{
	/*
	ppd->labels.push_back(Label(contact_point, [=] {
		char text[256];
		sprintf(text, "(%.6g, %.6g, %.6g)", contact_point.x, contact_point.y, contact_point.z);
		return std::string(text);
	}));*/

	
	Vector n = contact_distance.normalise();
	Vector rxn = contact_point.cross(n);

	Matrix I_1 = ppd->moment_of_inertia.inverse();

	double impulse = 2 * (rxn.dot(ppd->get_angular_vel()) + n.dot(ppd->vel))/(1/ppd->mass + (I_1*rxn).dot(rxn));

	ppd->vel = 0.99 * (-impulse/ppd->mass * n + ppd->vel);
	ppd->set_angular_vel(0.99 * (ppd->get_angular_vel() - impulse * I_1 * rxn));

	ppd->pos -= contact_distance; //.modulus() * wall->a.cross(wall->b).normalise();
}

void Collider::detect_and_collide(Ball* b1, Ball* b2)
{
	Vector distance = b1->pos - b2->pos;
	double entanglement_depth = b1->radius + b2->radius - distance.modulus();

	if(entanglement_depth > 0) collide(b1, b2, entanglement_depth);
}

void Collider::detect_and_collide(Ball* ball, Parallelepiped* ppd)
{
	const Vector ball_relpos = ball->pos - ppd->pos;
	
	const Vector a = ppd->a;
	const Vector b = ppd->b;
	const Vector c = ppd->c;
	const Vector a2 = ppd->a/2;
	const Vector b2 = ppd->b/2;
	const Vector c2 = ppd->c/2;
	
	Vector ball_relpos_princ = ppd->base_change * ball_relpos;

	const double A = ball_relpos_princ.x, B = ball_relpos_princ.y, C = ball_relpos_princ.z;

	const double aA = std::abs(A), aB = std::abs(B), aC = std::abs(C);

	auto check_and_collide = [&](Vector candidate)
	{
		Vector contact_distance = ball_relpos - candidate;

		if(aA < 0.5 && aB < 0.5 && aC < 0.5)
		{
			collide(ball, ppd, ball_relpos, -contact_distance);

			return true;
		}

		if(contact_distance.modulus() < ball->radius)
		{
			collide(ball, ppd, ball_relpos, contact_distance);

			return true;
		}
		return false;
	};

	Vector contact_candidate, contact_distance;

	

	

	// inside
	if(aA < 0.5 && aB < 0.5 && aC < 0.5)
	{
		//std::cout << ball_relpos_princ << std::endl;
	}

	
	
	// top and bottom
	if(aA < 0.5 && aC < 0.5)
	{
		if(check_and_collide(A * a + C * c + b2)) return; // top
		if(check_and_collide(A * a + C * c - b2)) return; // bottom
	}



	// right and left
	if(aB < 0.5 && aC < 0.5)
	{
		if(check_and_collide(B * b + C * c + a2)) return; // right
		if(check_and_collide(B * b + C * c - a2)) return; // left
	}


	// back and front
	if(aA < 0.5 && aB < 0.5)
	{
		if(check_and_collide(A * a + B * b + c2)) return; // back
		if(check_and_collide(A * a + B * b - c2)) return; // front
	}
	

	// now for the edges

	// parallel to A
	if(aA < 0.5)
	{
		if(check_and_collide(A * a + b2 + c2)) return;
		if(check_and_collide(A * a + b2 - c2)) return;
		if(check_and_collide(A * a - b2 - c2)) return;
		if(check_and_collide(A * a - b2 + c2)) return;
	}

	// parallel to B
	if(aB < 0.5)
	{
		if(check_and_collide(B * b + a2 + c2)) return;
		if(check_and_collide(B * b + a2 - c2)) return;
		if(check_and_collide(B * b - a2 - c2)) return;
		if(check_and_collide(B * b - a2 + c2)) return;
	}

	// parallel to C
	if(aC < 0.5)
	{
		if(check_and_collide(C * c + a2 + b2)) return;
		if(check_and_collide(C * c + a2 - b2)) return;
		if(check_and_collide(C * c - a2 - b2)) return;
		if(check_and_collide(C * c - a2 + b2)) return;
	}
	
}


void Collider::detect_and_collide(Ball* ball, Wall* wall)
{
	Vector ball_relpos = ball->pos - wall->pos;

	const double am = wall->a.modulus();
	const double bm = wall->b.modulus();
	const Vector a2 = wall->a/2;
	const Vector b2 = wall->b/2;
	const Vector an = wall->a/am;
	const Vector bn = wall->b/bm;

	Vector contact_candidate;

	Vector b_orthog_a = bn - an * bn.dot(an);

	const double B = b_orthog_a.dot(ball_relpos)/b_orthog_a.modulus2();
	const double A = an.dot(ball_relpos) - bn.dot(an) * B;

	contact_candidate = A * an + B * bn;
	
	if((contact_candidate - ball_relpos).modulus() <= ball->radius && abs(A) <= am/2 && abs(B) <= bm/2)
	{
		return collide(ball, wall, ball_relpos - contact_candidate);
	}

	contact_candidate = A * an - b2;

	if((contact_candidate - ball_relpos).modulus() <= ball->radius && abs(A) <= am/2)
	{
		return collide(ball, wall, ball_relpos - contact_candidate);
	}

	contact_candidate = A * an + b2;

	if((contact_candidate - ball_relpos).modulus() <= ball->radius && abs(A) <= am/2)
	{
		return collide(ball, wall, ball_relpos - contact_candidate);
	}

	contact_candidate = B * bn - a2;

	if((contact_candidate - ball_relpos).modulus() <= ball->radius && abs(B) <= bm/2)
	{
		return collide(ball, wall, ball_relpos - contact_candidate);
	}

	contact_candidate = B * bn + a2;

	if((contact_candidate - ball_relpos).modulus() <= ball->radius && abs(B) <= bm/2)
	{
		return collide(ball, wall, ball_relpos - contact_candidate);
	}
}

void Collider::detect_and_collide(Parallelepiped* ppd, Wall* wall)
{
	Vector contact_candidate, contact_rel_to_ppd, contact_rel_to_ppd_princ;

	double A, B;

	const double am = wall->a.modulus();
	const double bm = wall->b.modulus();
	const Vector an = wall->a/am;
	const Vector bn = wall->b/bm;

	Vector b_orthog_a = bn - an * bn.dot(an);


	B = b_orthog_a.dot(ppd->pos - wall->pos)/b_orthog_a.modulus2();
	A = an.dot(ppd->pos - wall->pos) - bn.dot(an) * B;
	Vector ppd_center_wall_proj = ppd->pos - (A * an + B * bn + wall->pos);


	Vector collision_corner = 0, collision_entanglement = 0;
	bool has_candidate = false;

	for(Vector corner : ppd->get_corners())
	{
		Vector corner_relpos = corner + ppd->pos - wall->pos;

		B = b_orthog_a.dot(corner_relpos)/b_orthog_a.modulus2();
		A = an.dot(corner_relpos) - bn.dot(an) * B;

		contact_candidate = A * an + B * bn;

		/*
		wall->labels.push_back(Label(contact_candidate, [=] {
			char text[256];
			sprintf(text, "(%.6g, %.6g, %.6g)", contact_candidate.x, contact_candidate.y, contact_candidate.z);
			return std::string(text);
		}));
		*/

		contact_rel_to_ppd = contact_candidate + wall->pos - ppd->pos;
		contact_rel_to_ppd_princ = ppd->base_change * contact_rel_to_ppd;

		Vector entanglement = corner - contact_rel_to_ppd;

		if(fabs(A) < am/2 && fabs(B) < bm/2 && ppd_center_wall_proj.dot(entanglement) < 0)
		{
			if(!has_candidate || entanglement.modulus() < collision_entanglement.modulus())
			{
				collision_entanglement = entanglement;
				collision_corner = corner;
				
			}

			has_candidate = true;
			//return collide(ppd, wall, corner, entanglement);
		}
	}

	
	if(has_candidate)
	{
		collide(ppd, wall, collision_corner, 2*collision_entanglement);
	}
}