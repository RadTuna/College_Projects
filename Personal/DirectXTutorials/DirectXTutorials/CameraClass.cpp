
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

	// Up벡터를 초기화 함.
	UpVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// 월드 좌표계에서의 카메라의 위치를 초기화.
	PositionVector = DirectX::XMVectorSet(mPositionX, mPositionY, mPositionZ, 0.0f);

	// 카메라가 바라볼 위치의 기본값 지정.
	LookAtVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Yaw, Pitch, Roll의 값을 라디안으로 저장.
	Pitch = DirectX::XMConvertToRadians(mRotationX);
	Yaw = DirectX::XMConvertToRadians(mRotationY);
	Roll = DirectX::XMConvertToRadians(mRotationZ);

	// Pitch, Yaw, Roll의 값을 사용해서 회전행렬을 생성.
	RotationMat = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	// Up벡터와 LookAt벡터를 회전행렬과 연산해 수정함.
	UpVector = DirectX::XMVector3TransformCoord(UpVector, RotationMat);
	LookAtVector = DirectX::XMVector3TransformCoord(LookAtVector, RotationMat);

	// 카메라의 위치에 맞게 LookAt벡터를 수정함.
	LookAtVector = DirectX::XMVectorAdd(PositionVector, LookAtVector);

	// 3개의 벡터를 이용해서 View행렬을 생성.
	DirectX::XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector));

	return;
}

DirectX::XMMATRIX CameraClass::GetViewMatrix() const
{
	return DirectX::XMLoadFloat4x4(&mViewMatrix);
}

