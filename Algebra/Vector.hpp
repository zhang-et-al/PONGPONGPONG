#pragma once

#include <math.h>
#include <ostream>

struct Vector
{
	double x, y, z;

	Vector(double _x = 0, double _y = 0, double _z = 0)
	 : x(_x), y(_y), z(_z) { }
	
	Vector(const Vector& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	
	bool operator==(const Vector& v) const
	{
		return fabs(x - v.x) < 1e-9 && fabs(y - v.y) < 1e-9 && fabs(z - v.z) < 1e-9;
	}

	// For ordering purposes only. Holds little mathematical meaning that I'm aware of.
	bool operator<(const Vector& v) const
	{
		if(fabs(x - v.x) > 1e-9) return x < v.x;
		if(fabs(x - v.x) > 1e-9) return y < v.y;
		return z < v.z;
	}
	bool operator>(const Vector& v) const
	{
		if(fabs(x - v.x) > 1e-9) return x > v.x;
		if(fabs(x - v.x) > 1e-9) return y > v.y;
		return z > v.z;
	}
	
	Vector operator+(const Vector& v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	inline void operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	Vector operator-() const
	{
		return Vector(-x, -y, -z);
	}
	Vector operator-(const Vector& v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	inline void operator-=(const Vector& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	
	Vector operator*(const Vector& v) const
	{
		return Vector(x * v.x, y * v.y, z * v.z);
	}
	inline void operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}
	
	Vector operator*(double t) const
	{
		return Vector(x * t, y * t, z * t);
	}
	inline void operator*=(double t)
	{
		x *= t;
		y *= t;
		z *= t;
	}
	
	Vector operator/(const Vector& v) const
	{
		return Vector(x / v.x, y / v.y, z / v.z);
	}
	inline void operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}
	Vector operator/(double t) const
	{
		Vector vec(x / t, y / t, z / t);
		return vec;
	}
	inline void operator/=(double t)
	{
		x /= t;
		y /= t;
		z /= t;
	}
	
	inline double modulus() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	inline double modulus2() const
	{
		return x*x + y*y + z*z;
	}
	
	inline double dot(const Vector& vec) const
	{
		return x*vec.x + y*vec.y + z*vec.z;
	}
	
	inline Vector cross(const Vector& vec) const
	{
		return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - vec.x*y);
	}

	inline Vector normalise() const
	{
		return *this / modulus();
	}
	
	// Rotates the vector horizontally (azimuth angle)
	void rotate_azimuth(double angle)
	{
		double former_x, s, c;
		
		// Since both of these values are to be used twice, we work them out before hand to avoid excess computations
		s = sin(angle);
		c = cos(angle);
		
		former_x = x;
		
		x = x * c + z * s;
		z = z * c - former_x * s;
	}
	void rotate_azimuth(double sin_angle, double cos_angle)
	{
		double former_x = x;
		
		x = x * cos_angle + z * sin_angle;
		z = z * cos_angle - former_x * sin_angle;
	}
	// Rotates the vector vertically (altitude angle) around the point in center_z, center_y
	void rotate_altitude(double angle)
	{
		double former_z, s, c;
		
		// Since both of these values are to be used twice, we work them out before hand to avoid excess computations
		s = sin(angle);
		c = cos(angle);
		
		former_z = z;
		
		z = z * c + y * s;
		y = y * c - former_z * s;
	}
	void rotate_altitude(double sin_angle, double cos_angle)
	{
		double former_z = z;
		
		z = z * cos_angle + y * sin_angle;
		y = y * cos_angle - former_z * sin_angle;
	}
	
	double get_altitude() const
	{
		return atan2(hypot(x, z), y);
	}
	double get_azimuth() const
	{
		return atan2(x, z);
	}

	void rotate_x(double a)
	{
		double former, s, c;
		former = y;
		s = sin(a);
		c = cos(a);
		y = y * c - z * s;
		z = z * c + former * s;
	}
	void rotate_x(double s, double c)
	{
		double former = y;
		y = y * c - z * s;
		z = z * c + former * s;
	}

	void rotate_y(double a)
	{
		double former, s, c;
		former = x;
		s = sin(a);
		c = cos(a);
		x = x * c + z * s;
		z = z * c - former * s;
	}
	void rotate_y(double s, double c)
	{
		double former = x;
		x = x * c + z * s;
		z = z * c - former * s;
	}

	void rotate_z(double a)
	{
		double former, s, c;
		former = x;
		s = sin(a);
		c = cos(a);
		x = x * c - y * s;
		y = y * c + former * s;
	}
	void rotate_z(double s, double c)
	{
		double former = x;
		x = x * c - y * s;
		y = y * c + former * s;
	}

	void rotate(Vector n, double angle)
	{
		*this = (*this)*cos(angle) + (n.cross(*this)) * sin(angle) + n*(n.dot(*this))*(1-cos(angle));
	}

	void rotate(Vector axis)
	{
		/*	
		double sx = sin(axis.x);
		double cx = cos(axis.x);
		double sy = sin(axis.y);
		double cy = cos(axis.y);
		double sz = sin(axis.z);
		double cz = cos(axis.z);
		double _x = x, _y = y;

		x = x*cy*cz 	+ y*(sx*sy*cz - cx*sz)	+ z*(cx*sy*cz + sx*sz);
		y = _x*cy*sz	+ y*(sx*sy*sz + cx*cz)	+ z*(cx*sy*sz - sx*cz);
		z = -_x*sy		+ _y*sx*cy				+ z*cx*cy;
		*/

		double mod = axis.modulus();
		if(mod == 0) return;
		rotate(axis/mod, mod);
	}

	Vector angle_to(Vector v) const
	{
		return cross(v).normalise() * acos(dot(v)/(modulus()*v.modulus()));
	}

	Vector operator[](int j) const
	{
		switch(j)
		{
			case 1:
				return x;
				break;
			case 2:
				return y;
				break;
			case 3:
				return z;
				break;
			default:
				return 0;
		}
	}

	bool zero() const
	{
		return x == 0.0 && y == 0.0 && z == 0.0;
	}
};

// Previoulsy defined operators but in inverse order
inline Vector operator*(double t, Vector vec)
{
	return Vector(vec.x * t, vec.y * t, vec.z * t);;
}
inline void operator*=(double t, Vector vec)
{
	vec.x *= t;
	vec.y *= t;
	vec.z *= t;
}

inline Vector operator/(double t, Vector vec)
{
	return Vector(vec.x / t, vec.y / t, vec.z / t);
}
inline void operator/=(double t, Vector vec)
{
	vec.x /= t;
	vec.y /= t;
	vec.z /= t;
}

inline std::ostream& operator<<(std::ostream& os, Vector v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
	return os;
}