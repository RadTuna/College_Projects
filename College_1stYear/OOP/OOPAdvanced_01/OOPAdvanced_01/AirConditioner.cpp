#include <iostream>
#include "AirConditioner.h"


AirConditioner::AirConditioner()
{
	CurrentTemp = 18.0f;
	IsPowerOn = false;
}

void AirConditioner::TogglePower()
{
	if (IsPowerOn == true)
	{
		IsPowerOn = false;
	}
	else
	{
		IsPowerOn = true;
	}
}

void AirConditioner::SetTemp(float inTemp)
{
	if (IsPowerOn == false)
	{
		std::cout << "The power is off." << std::endl;
		return;
	}

	CurrentTemp = inTemp;
}

float AirConditioner::GetTemp()
{
	if (IsPowerOn == false)
	{
		std::cout << "The power is off." << std::endl;
		return 0.0f;
	}

	return CurrentTemp;
}

void AirConditioner::PrintTemp()
{
	if (IsPowerOn == false)
	{
		std::cout << "The power is off." << std::endl;
		return;
	}

	std::cout << "CurrentTemp : " << CurrentTemp << std::endl;
}

void AirConditioner::TempUpOne()
{
	if (IsPowerOn == false)
	{
		std::cout << "The power is off." << std::endl;
		return;
	}

	CurrentTemp++;
}

void AirConditioner::TempDownOne()
{
	if (IsPowerOn == false)
	{
		std::cout << "The power is off." << std::endl;
		return;
	}

	CurrentTemp--;
}