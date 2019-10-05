#include "Car.h"

Car::Car(int InGas)
	: gasolineGauge(InGas)
{
}

Car::~Car()
{
}

int Car::GetGasGauge()
{
	return gasolineGauge;
}
