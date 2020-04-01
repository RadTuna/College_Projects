#pragma once

#include "Vehicle.h"

class Plane : public Vehicle
{
public:

	Plane(int inFlight, int inSpeed);
	virtual ~Plane();

	virtual void Show() override;

private:

	int flight;

};

