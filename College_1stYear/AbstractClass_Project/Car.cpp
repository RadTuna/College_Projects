#include "Car.h"
#include <iostream>

Car::Car(int inNum, double inGas, int inSpeed)
	: Vehicle(inSpeed),
	num(inNum),
	gas(inGas)
{
}

Car::~Car()
{
}

void Car::Show()
{
	std::cout << "자동차" << std::endl;
	std::cout << "번호 : " << num << ", 가솔린양 : " << gas << ", 속도 : " << speed << std::endl;
}
