#include "Point.h"

Point::Point()
	: X(0),
	Y(0)
{
}

Point::Point(float InX, float InY)
	: X(InX),
	Y(InY)
{
}

Point::~Point()
{
}

void Point::SetX(float InX)
{
	X = InX;
}

void Point::SetY(float InY)
{
	Y = InY;
}
