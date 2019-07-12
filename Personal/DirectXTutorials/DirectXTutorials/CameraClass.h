
#pragma once


#include <DirectXMath.h>


class CameraClass
{
public:

	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPostion() const;
	DirectX::XMFLOAT3 GetRotation() const;

	void Render();
	DirectX::XMMATRIX GetViewMatrix() const;

private:

	float mPositionX;
	float mPositionY;
	float mPositionZ;

	float mRotationX;
	float mRotationY;
	float mRotationZ;

	DirectX::XMMATRIX mViewMatrix;

};
