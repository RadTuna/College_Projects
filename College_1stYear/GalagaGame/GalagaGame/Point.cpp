#include "Point.h"

Point::Point()
	: X(0)
	, Y(0)
{
}

Point::Point(int InX, int InY)
	: X(InX)
	, Y(InY)
{
}


void Point::SetPoint(int InX, int InY)
{
	X = InX;
	Y = InY;
}
