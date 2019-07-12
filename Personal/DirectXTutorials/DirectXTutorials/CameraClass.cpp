
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

	// Up벡터를 초기화 함.
	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.z = 0.0f;

	UpVector = DirectX::XMLoadFloat3(&Up);

	// 월드 좌표계에서의 카메라의 위치를 초기화.
	Position.x = mPositionX;
	Position.y = mPositionY;
	Position.z = mPositionZ;

	PositionVector = DirectX::XMLoadFloat3(&Position);

	// 카메라가 바라볼 위치의 기본값 지정.
	LookAt.x = 0.0f;
	LookAt.y = 0.0f;
	LookAt.z = 1.0f;

	LookAtVector = DirectX::XMLoadFloat3(&LookAt);

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
	mViewMatrix = DirectX::XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector);

	return;
}

DirectX::XMMATRIX CameraClass::GetViewMatrix() const
{
	return mViewMatrix;
}

