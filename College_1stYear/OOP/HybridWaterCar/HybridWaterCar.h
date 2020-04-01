#pragma once
#include "HybridCar.h"
class HybridWaterCar : public HybridCar
{
public:

	HybridWaterCar(int InGas, int InElec, int InWater);
	~HybridWaterCar();

	void ShowCurrentGauge();

private:

	int waterGauge;

};

