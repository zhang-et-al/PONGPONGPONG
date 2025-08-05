#include "Arrow.hpp"
#include "../Algebra/Matrix.hpp"

#include <math.h>
#include <iostream>

Mesh Arrow::preset;

Vector Arrow::get_direction() const
{
	return direction;
}

void Arrow::set_direction(const Vector& dir)
{
	//Matrix rotation = Matrix::Rotation(direction.angle_to(dir));

	direction = dir;

	//mesh.clear();

	Mesh new_mesh;


	for(const Triangle& tri : preset)
	{
		Triangle triangle = tri * Vector(radius, direction.modulus(), radius);

		triangle.rotate_altitude(direction.get_altitude());
		triangle.rotate_azimuth(direction.get_azimuth());
		triangle.color = color;
		
		new_mesh.add(triangle);
	}

	mesh = new_mesh;
}

void Arrow::setup_mesh()
{

	if(preset.empty())
	{
		Vector tip(0, 1, 0);

		float delta = M_PI / 12;

		for(float theta = 0; theta < 2 * M_PI; theta += delta)
		{
			Vector outer_circ_top = Vector(cos(theta), 0.75, sin(theta));
			Vector outer_circ_top_next = Vector(cos(theta + delta), 0.75, sin(theta + delta));

			Vector inner_circ_top = Vector(0.5, 1, 0.5) * outer_circ_top;
			Vector inner_circ_top_next = Vector(0.5, 1, 0.5) * outer_circ_top_next;

			Vector circ_bottom = inner_circ_top - Vector(0, 0.75, 0);
			Vector circ_bottom_next = inner_circ_top_next - Vector(0, 0.75, 0);

			// Cone
			preset.add(outer_circ_top, outer_circ_top_next, tip, color);
			
			// Cone to cylinder
			preset.add(outer_circ_top, outer_circ_top_next, inner_circ_top, color);
			preset.add(inner_circ_top, outer_circ_top_next, inner_circ_top_next, color);

			// Cylinder
			Triangle t1(inner_circ_top, inner_circ_top_next, circ_bottom, color);
			Triangle t2(circ_bottom_next, inner_circ_top_next, circ_bottom, color);
			t1 *= Vector(1, 0.2, 1);
			t2 *= Vector(1, 0.2, 1);
			for(float i = 0; i < 0.12; i+=0.01)
			{
				preset.add(t1 + Vector(0, i * 5, 0));
				preset.add(t2 + Vector(0, i * 5, 0));
			}

			// Cylinder bottom cap
			preset.add(circ_bottom_next, circ_bottom, 0, color);
		}
	}
	
	set_direction(direction);

	mass = 0;
}
