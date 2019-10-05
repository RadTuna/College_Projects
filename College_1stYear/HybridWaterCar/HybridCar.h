#pragma once
#include "Car.h"
class HybridCar : public Car
{
public:

	HybridCar(int InGas, int InElec);
	~HybridCar();

	int GetElecGauge();

private:

	int electricGauge;

};

