// A 2,1-D base for a 2,1-D vector space
class Base
{
	Vector a, b;

	// Projects a vector to the vector subspace defined by this base. Doesn't have to be orthogonal
	Vector project(Vector v);


	//Vector from_canonical(Vector v);
};