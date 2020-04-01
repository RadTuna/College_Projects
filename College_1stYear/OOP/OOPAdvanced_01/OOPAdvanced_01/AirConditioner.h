#pragma once

class AirConditioner
{
public:

	AirConditioner();
	~AirConditioner() = default;

	void TogglePower();
	void SetTemp(float inTemp);
	float GetTemp();
	void PrintTemp();
	void TempUpOne();
	void TempDownOne();

private:

	float CurrentTemp;
	bool IsPowerOn;

};
