#pragma once

#include "Shape.hpp"

class Visor : public Shape
{
public:
	Visor(const Vector& p = 0, const Vector& v = 0);

	bool rugged = false;
	
	float get_azimuth();
	void set_azimuth(float azimuth);

	float get_altitude();
	void set_altitude(float altitude);

	double sin_azimuth = 0.0998;
	double cos_azimuth = 0.995;
	double sin_altitude = 0.0998;
	double cos_altitude = 0.995;

	Type type() const
	{
		return Type::visor;
	}

private:
	void setup_mesh() { }

	double azimuth = 1e-9;
	double altitude = 1e-9;
};
