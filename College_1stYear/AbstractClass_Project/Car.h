#pragma once

#include "Vehicle.h"

class Car : public Vehicle
{
public:

	Car(int inNum, double inGas, int inSpeed);
	virtual ~Car();

	virtual void Show() override;

private:

	int num;
	double gas;

};
