#pragma once

#include "Vector.hpp"

#include <string>

// Yes, I hardcoded everything because I will only be needing 3x3 matrices and a high performance.
struct Matrix
{
	double m11, m12, m13, m21, m22, m23, m31, m32, m33;
	
	Matrix(double a11 = 0, double a12 = 0, double a13 = 0, double a21 = 0, double a22 = 0, double a23 = 0, double a31 = 0, double a32 = 0, double a33 = 0)
	: m11(a11), m12(a12), m13(a13), m21(a21), m22(a22), m23(a23), m31(a31), m32(a32), m33(a33) {}

	static Matrix I()
	{
		return Matrix(
			1,0,0,
			0,1,0,
			0,0,1
		);
	}

	static Matrix Rotation(Vector n, double angle)
	{
		Matrix K = Matrix(
			0,		-n.z,	n.y,
			n.z,	0	,	-n.x,
			-n.y,	n.x	,	0
		);

		return I() + K * sin(angle) + (K*K)*(1-cos(angle));
	}

	static Matrix Rotation(Vector axis)
	{
		double mod = axis.modulus();
		if(mod == 0) return I();
		return Rotation(axis/mod, mod);
		/*
		double sx = sin(axis.x);
		double cx = cos(axis.x);
		double sy = sin(axis.y);
		double cy = cos(axis.y);
		double sz = sin(axis.z);
		double cz = cos(axis.z);

		return Matrix(
			cy*cz 	, sx*sy*cz - cx*sz	, cx*sy*cz + sx*sz,
			cy*sz	, sx*sy*sz + cx*cz	, cx*sy*sz - sx*cz,
			-sy		, sx*cy				, cx*cy
		);
		*/
	}

	static Matrix Columns(Vector v1, Vector v2, Vector v3)
	{
		return Matrix(
			v1.x, v2.x, v3.x,
			v1.y, v2.y, v3.y,
			v1.z, v2.z, v3.z
		);
	}
	
	double det() const
	{
		return m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - m13*m22*m31 - m12*m21*m33 - m11*m32*m23;
	}

	Matrix cofactor() const
	{
		return Matrix(
			m22*m33-m23*m32, m23*m31-m21*m33, m21*m32-m22*m31,
			m13*m32-m12*m33, m11*m33-m13*m31, m12*m31-m11*m32,
			m12*m23-m22*m13, m13*m21-m11*m23, m11*m22-m12*m21
		);
	}

	Matrix adjoint() const
	{
		return cofactor().transpose();
	}

	Matrix transpose() const
	{
		return Matrix(m11, m21, m31, m12, m22, m32, m13, m23, m33);
	}

	Matrix inverse() const
	{
		return adjoint()/det();
	}

	Vector operator*(Vector v) const
	{
		return Vector(m11*v.x+m12*v.y+m13*v.z, m21*v.x+m22*v.y+m23*v.z, m31*v.x+m32*v.y+m33*v.z);
	}

	Matrix operator*(double t) const
	{
		return Matrix(m11*t, m12*t, m13*t, m21*t, m22*t, m23*t, m31*t, m32*t, m33*t);
	}

	Matrix operator/(double t) const
	{
		return Matrix(m11/t, m12/t, m13/t, m21/t, m22/t, m23/t, m31/t, m32/t, m33/t);
	}

	Matrix operator*(Matrix n) const
	{
		return Matrix(
			m11*n.m11+m12*n.m21+m13*n.m31, m11*n.m12+m12*n.m22+m13*n.m32, m11*n.m13+m12*n.m23+m13*n.m33,
			m21*n.m11+m22*n.m21+m23*n.m31, m21*n.m12+m22*n.m22+m23*n.m32, m21*n.m13+m22*n.m23+m23*n.m33,
			m31*n.m11+m32*n.m21+m33*n.m31, m31*n.m12+m32*n.m22+m33*n.m32, m31*n.m13+m32*n.m23+m33*n.m33
		);
	}

	Matrix operator+(Matrix n) const
	{
		return Matrix(
			m11+n.m11, m12+n.m12, m13+n.m13,
			m21+n.m21, m22+n.m22, m23+n.m23,
			m31+n.m31, m32+n.m32, m33+n.m33
		);
	}

	Vector operator[](int i) const
	{
		switch(i)
		{
			case 1:
				return Vector(m11, m21, m31);
				break;
			case 2:
				return Vector(m12, m22, m32);
				break;
			case 3:
				return Vector(m13, m23, m33);
				break;
			default:
				return 0;
		}
	}

	Matrix rotate(Vector axis) const
	{
		return Matrix::Rotation(axis) * (*this) * Matrix::Rotation(axis).transpose();
	}

	
	std::string dump() const;
};

inline Matrix operator*(double t, Matrix mat)
{
	return mat * t;
}

inline Matrix operator/(double t, Matrix mat)
{
	return mat / t;
}