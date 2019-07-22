
#include "LightClass.h"


LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass& Other)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float Red, float Green, float Blue, float Alpha)
{
	mAmbientColor = DirectX::XMFLOAT4(Red, Green, Blue, Alpha);
	return;
}

void LightClass::SetDiffuseColor(float Red, float Green, float Blue, float Alpha)
{
	mDiffuseColor = DirectX::XMFLOAT4(Red, Green, Blue, Alpha);
	return;
}

void LightClass::SetDirection(float X, float Y, float Z)
{
	mDirection = DirectX::XMFLOAT3(X, Y, Z);
	return;
}

void LightClass::SetSpecularColor(float Red, float Green, float Blue, float Alpha)
{
	mSpecularColor = DirectX::XMFLOAT4(Red, Green, Blue, Alpha);
	return;
}

void LightClass::SetSpecularPower(float Power)
{
	mSpecularPower = Power;
	return;
}

