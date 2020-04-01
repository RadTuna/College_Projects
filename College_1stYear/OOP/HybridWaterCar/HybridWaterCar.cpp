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
	std::cout << "�ܿ����ָ� : " << GetGasGauge() << std::endl;
	std::cout << "�ܿ����ⷮ : " << GetElecGauge() << std::endl;
	std::cout << "�ܿ����ͷ� : " << waterGauge << std::endl;

	return;
}
