#include "Matrix.hpp"

#include <sstream>

std::string Matrix::dump() const
{
	std::ostringstream strs;
	strs << m11 << "\t" << m12 << "\t" << m13 << "\n"
		 << m21 << "\t" << m22 << "\t" << m23 << "\n"
		 << m31 << "\t" << m32 << "\t" << m33 << "\n";


	return strs.str();
}