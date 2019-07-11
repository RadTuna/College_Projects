
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

	DirectX::XMVECTOR GetPostion() const;
	DirectX::XMVECTOR GetRotation() const;

	void Render();
	DirectX::XMMATRIX GetViewMatrix() const;

private:

	float mPositionX;
	float mPositionY;
	float mPositionZ;

	float mRotationX;
	float mRotationY;
	float mRotationZ;

	DirectX::XMFLOAT4X4 mViewMatrix;

};
