#pragma once

class Vehicle
{
public:

	Vehicle(int inSpeed);
	virtual ~Vehicle();

	virtual void Show() = 0;

protected:

	int speed;

};
