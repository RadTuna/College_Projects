#include "HybridCar.h"

HybridCar::HybridCar(int InGas, int InElec)
	: Car(InGas),
	electricGauge(InElec)
{
}

HybridCar::~HybridCar()
{
}

int HybridCar::GetElecGauge()
{
	return electricGauge;
}
