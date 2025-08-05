#include "Visor.hpp"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <iostream>


Visor::Visor(const Vector& p, const Vector& v)
: Shape(p, v, 0, 0, Color::white, 1, false)
{
	mass = 0;
	frict_consts = Vector(1.5, 1.5, 1.5);
}

float Visor::get_azimuth()
{
	return azimuth;
}
void Visor::set_azimuth(float az)
{
	azimuth = az;
	sin_azimuth = sin(azimuth);
	cos_azimuth = cos(azimuth);
}

float Visor::get_altitude()
{
	return altitude;
}
void Visor::set_altitude(float al)
{
	altitude = al;
	sin_altitude = sin(altitude);
	cos_altitude = cos(altitude);
}
