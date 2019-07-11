
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
	using namespace DirectX;

	DirectX::XMVECTOR UP;
	DirectX::XMVECTOR Position;
	DirectX::XMVECTOR LookAt;

	float Yaw;
	float Picth;
	float Roll;

	DirectX::XMMATRIX RotationMat;

	// Up���͸� �ʱ�ȭ ��.
	UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// ���� ��ǥ�迡���� ī�޶��� ��ġ�� �ʱ�ȭ.
	Position = DirectX::XMVectorSet(mRotationX, mRotationY, mRotationZ, 0.0f);

	// ī�޶� �ٶ� ��ġ�� �⺻�� ����.
	LookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Yaw, Pitch, Roll�� ���� �������� ����.
	Picth = DirectX::XMConvertToRadians(mRotationX);
	Yaw = DirectX::XMConvertToRadians(mRotationY);
	Roll = DirectX::XMConvertToRadians(mRotationZ);

	// Pitch, Yaw, Roll�� ���� ����ؼ� ȸ������� ����.
	RotationMat = DirectX::XMMatrixRotationRollPitchYaw(Picth, Yaw, Roll);

	// Up���Ϳ� LookAt���͸� ȸ����İ� ������ ������.
	UP = DirectX::XMVector3TransformCoord(UP, RotationMat);
	LookAt = DirectX::XMVector3TransformCoord(LookAt, RotationMat);

	// ī�޶��� ��ġ�� �°� LookAt���͸� ������.
	LookAt = LookAt + Position;

	// 3���� ���͸� �̿��ؼ� View����� ����.
	DirectX::XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixLookAtLH(Position, LookAt, UP));

	return;
}

DirectX::XMMATRIX CameraClass::GetViewMatrix() const
{
	return DirectX::XMLoadFloat4x4(&mViewMatrix);
}

