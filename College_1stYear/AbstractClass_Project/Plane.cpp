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
	std::cout << "�����" << std::endl;
	std::cout << "��� : " << flight << ", �ӵ� : " << speed << std::endl;
}
