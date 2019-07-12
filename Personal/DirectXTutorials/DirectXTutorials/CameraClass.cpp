
#include "CameraClass.h"


CameraClass::CameraClass()
{
	mPositionX = mPositionY = mPositionZ = 0.0f;
	mRotationX = mRotationY = mRotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& Other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float X, float Y, float Z)
{
	mPositionX = X;
	mPositionY = Y;
	mPositionZ = Z;

	return;
}

void CameraClass::SetRotation(float X, float Y, float Z)
{
	mRotationX = X;
	mRotationY = Y;
	mRotationZ = Z;

	return;
}

DirectX::XMVECTOR CameraClass::GetPostion() const
{
	return DirectX::XMVectorSet(mPositionX, mPositionY, mPositionZ, 0.0f);
}

DirectX::XMVECTOR CameraClass::GetRotation() const
{
	return DirectX::XMVectorSet(mRotationX, mRotationY, mRotationZ, 0.0f);
}

void CameraClass::Render()
{
	DirectX::XMVECTOR UpVector, PositionVector, LookAtVector;

	float Yaw;
	float Pitch;
	float Roll;

	DirectX::XMMATRIX RotationMat;

	// Up���͸� �ʱ�ȭ ��.
	UpVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// ���� ��ǥ�迡���� ī�޶��� ��ġ�� �ʱ�ȭ.
	PositionVector = DirectX::XMVectorSet(mPositionX, mPositionY, mPositionZ, 0.0f);

	// ī�޶� �ٶ� ��ġ�� �⺻�� ����.
	LookAtVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Yaw, Pitch, Roll�� ���� �������� ����.
	Pitch = DirectX::XMConvertToRadians(mRotationX);
	Yaw = DirectX::XMConvertToRadians(mRotationY);
	Roll = DirectX::XMConvertToRadians(mRotationZ);

	// Pitch, Yaw, Roll�� ���� ����ؼ� ȸ������� ����.
	RotationMat = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	// Up���Ϳ� LookAt���͸� ȸ����İ� ������ ������.
	UpVector = DirectX::XMVector3TransformCoord(UpVector, RotationMat);
	LookAtVector = DirectX::XMVector3TransformCoord(LookAtVector, RotationMat);

	// ī�޶��� ��ġ�� �°� LookAt���͸� ������.
	LookAtVector = DirectX::XMVectorAdd(PositionVector, LookAtVector);

	// 3���� ���͸� �̿��ؼ� View����� ����.
	DirectX::XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector));

	return;
}

DirectX::XMMATRIX CameraClass::GetViewMatrix() const
{
	return DirectX::XMLoadFloat4x4(&mViewMatrix);
}

