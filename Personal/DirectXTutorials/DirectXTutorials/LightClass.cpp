
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

DirectX::XMVECTOR LightClass::GetDiffuseColor() const
{
	return DirectX::XMLoadFloat4(&mDiffuseColor);
}

DirectX::XMVECTOR LightClass::GetDirection() const
{
	return DirectX::XMLoadFloat3(&mDirection);
}
