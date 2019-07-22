#pragma once

#include <DirectXMath.h>


class LightClass
{
public:

	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

private:

	DirectX::XMFLOAT4 mAmbientColor;
	DirectX::XMFLOAT4 mDiffuseColor;
	DirectX::XMFLOAT3 mDirection;
	DirectX::XMFLOAT4 mSpecularColor;
	float mSpecularPower;

public:

	inline DirectX::XMVECTOR GetAmbientColor() const { return DirectX::XMLoadFloat4(&mAmbientColor); }
	inline DirectX::XMVECTOR GetDiffuseColor() const { return DirectX::XMLoadFloat4(&mDiffuseColor); }
	inline DirectX::XMVECTOR GetDirection() const { return DirectX::XMLoadFloat3(&mDirection); }
	inline DirectX::XMVECTOR GetSpecularColor() const { return DirectX::XMLoadFloat4(&mSpecularColor); }
	inline float GetSpecularPower() const { return mSpecularPower; }

};

