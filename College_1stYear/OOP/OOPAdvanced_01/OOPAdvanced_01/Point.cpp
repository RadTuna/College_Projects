#include <iostream>
#include "Point.h"

void Point::SetPos(float inX, float inY)
{
	X = inX;
	Y = inY;
}

void Point::Show()
{
	std::cout << "PosX : " << X << std::endl
		<< "PosY : " << Y << std::endl;
}