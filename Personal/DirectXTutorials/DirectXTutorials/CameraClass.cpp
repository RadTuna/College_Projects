
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

DirectX::XMFLOAT3 CameraClass::GetPostion() const
{
	return DirectX::XMFLOAT3(mPositionX, mPositionY, mPositionZ);
}

DirectX::XMFLOAT3 CameraClass::GetRotation() const
{
	return DirectX::XMFLOAT3(mRotationX, mRotationY, mRotationZ);
}

void CameraClass::Render()
{
	DirectX::XMFLOAT3 Up, Position, LookAt;
	DirectX::XMVECTOR UpVector, PositionVector, LookAtVector;

	float Yaw;
	float Pitch;
	float Roll;

	DirectX::XMMATRIX RotationMat;

	// Up���͸� �ʱ�ȭ ��.
	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.z = 0.0f;

	UpVector = DirectX::XMLoadFloat3(&Up);

	// ���� ��ǥ�迡���� ī�޶��� ��ġ�� �ʱ�ȭ.
	Position.x = mPositionX;
	Position.y = mPositionY;
	Position.z = mPositionZ;

	PositionVector = DirectX::XMLoadFloat3(&Position);

	// ī�޶� �ٶ� ��ġ�� �⺻�� ����.
	LookAt.x = 0.0f;
	LookAt.y = 0.0f;
	LookAt.z = 1.0f;

	LookAtVector = DirectX::XMLoadFloat3(&LookAt);

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
	mViewMatrix = DirectX::XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector);

	return;
}

DirectX::XMMATRIX CameraClass::GetViewMatrix() const
{
	return mViewMatrix;
}

