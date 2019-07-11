
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

	// Up벡터를 초기화 함.
	UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// 월드 좌표계에서의 카메라의 위치를 초기화.
	Position = DirectX::XMVectorSet(mRotationX, mRotationY, mRotationZ, 0.0f);

	// 카메라가 바라볼 위치의 기본값 지정.
	LookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Yaw, Pitch, Roll의 값을 라디안으로 저장.
	Picth = DirectX::XMConvertToRadians(mRotationX);
	Yaw = DirectX::XMConvertToRadians(mRotationY);
	Roll = DirectX::XMConvertToRadians(mRotationZ);

	// Pitch, Yaw, Roll의 값을 사용해서 회전행렬을 생성.
	RotationMat = DirectX::XMMatrixRotationRollPitchYaw(Picth, Yaw, Roll);

	// Up벡터와 LookAt벡터를 회전행렬과 연산해 수정함.
	UP = DirectX::XMVector3TransformCoord(UP, RotationMat);
	LookAt = DirectX::XMVector3TransformCoord(LookAt, RotationMat);

	// 카메라의 위치에 맞게 LookAt벡터를 수정함.
	LookAt = LookAt + Position;

	// 3개의 벡터를 이용해서 View행렬을 생성.
	DirectX::XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixLookAtLH(Position, LookAt, UP));

	return;
}

DirectX::XMMATRIX CameraClass::GetViewMatrix() const
{
	return DirectX::XMLoadFloat4x4(&mViewMatrix);
}

