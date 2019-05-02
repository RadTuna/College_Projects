#include "Vector.h"

Vector::Vector(int X, int Y)
	: X(X)
	, Y(Y)
{
}

int Vector::GetX() const
{
	return X;
}

int Vector::GetY() const
{
	return Y;
}