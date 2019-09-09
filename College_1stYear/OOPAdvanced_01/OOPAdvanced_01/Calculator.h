#pragma once

class Calculator
{
public:

	Calculator() = default;
	~Calculator() = default;

	float Add(float aNum, float bNum);
	float Sub(float aNum, float bNum);
	float Mul(float aNum, float bNum);
	float Div(float aNum, float bNum);

};
