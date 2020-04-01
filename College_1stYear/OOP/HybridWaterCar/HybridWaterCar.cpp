#include "HybridWaterCar.h"
#include <iostream>

HybridWaterCar::HybridWaterCar(int InGas, int InElec, int InWater)
	: HybridCar(InGas, InElec),
	waterGauge(InWater)
{
}

HybridWaterCar::~HybridWaterCar()
{
}

void HybridWaterCar::ShowCurrentGauge()
{
	std::cout << "ÀÜ¿©°¡¼Ö¸° : " << GetGasGauge() << std::endl;
	std::cout << "ÀÜ¿©Àü±â·® : " << GetElecGauge() << std::endl;
	std::cout << "ÀÜ¿©¿öÅÍ·® : " << waterGauge << std::endl;

	return;
}
