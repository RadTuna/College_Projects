#include "Plane.h"
#include <iostream>

Plane::Plane(int inFlight, int inSpeed)
	: Vehicle(inSpeed),
	flight(inFlight)
{
}

Plane::~Plane()
{
}

void Plane::Show()
{
	std::cout << "비행기" << std::endl;
	std::cout << "편명 : " << flight << ", 속도 : " << speed << std::endl;
}
