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
	std::cout << "�ڵ���" << std::endl;
	std::cout << "��ȣ : " << num << ", ���ָ��� : " << gas << ", �ӵ� : " << speed << std::endl;
}
