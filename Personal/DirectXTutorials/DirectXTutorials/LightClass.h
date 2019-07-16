#pragma once

#include <DirectXMath.h>


class LightClass
{
public:

	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	DirectX::XMVECTOR GetDiffuseColor() const;
	DirectX::XMVECTOR GetDirection() const;

private:

	DirectX::XMFLOAT4 mDiffuseColor;
	DirectX::XMFLOAT3 mDirection;

};

