#include "TMath.h"


Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float InX, float InY)
{
	x = InX;
	y = InY;
}

Vector2 Vector2::operator-(const Vector2& Other) const
{
	Vector2 Temp;
	Temp.x = x - Other.x;
	Temp.y = y - Other.y;
	return Temp;
}

float Vector2::DotProduct(const Vector2& Other) const
{
	return x * Other.x + y * Other.y;
}

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float InX, float InY, float InZ)
{
	x = InX;
	y = InY;
	z = InZ;
}

ColorRGB::ColorRGB()
{
	Red = 0;
	Green = 0;
	Blue = 0;
}

ColorRGB::ColorRGB(BYTE InRed, BYTE InGreen, BYTE InBlue)
{
	Red = InRed;
	Green = InGreen;
	Blue = InBlue;
}

ColorRGB ColorRGB::operator*(const float& Other) const
{
	ColorRGB Temp;
	BYTE minByte = 0;
	BYTE maxByte = 255;

	Temp.Red = TMath::Clamp<BYTE>(static_cast<BYTE>(Red * Other), minByte, maxByte);
	Temp.Green = TMath::Clamp<BYTE>(static_cast<BYTE>(Green * Other), minByte, maxByte);
	Temp.Blue = TMath::Clamp<BYTE>(static_cast<BYTE>(Blue * Other), minByte, maxByte);
	return Temp;
}

ColorRGB ColorRGB::operator+(const ColorRGB& Other) const
{
	ColorRGB Temp;
	BYTE MinByte = 0;
	BYTE MaxByte = 255;

	Temp.Red = TMath::Clamp<BYTE>(static_cast<BYTE>(Red + Other.Red), MinByte, MaxByte);
	Temp.Green = TMath::Clamp<BYTE>(static_cast<BYTE>(Green + Other.Green), MinByte, MaxByte);
	Temp.Blue = TMath::Clamp<BYTE>(static_cast<BYTE>(Blue + Other.Blue), MinByte, MaxByte);
	return Temp;
}

Vector2 TMath::Vector2Set(float InX, float InY)
{
	Vector2 Temp;
	Temp.x = InX;
	Temp.y = InY;
	return Temp;
}

Vector3 TMath::Vector3Set(float InX, float InY, float InZ)
{
	Vector3 Temp;
	Temp.x = InX;
	Temp.y = InY;
	Temp.z = InZ;
	return Temp;
}

ColorRGB TMath::ColorSet(BYTE InRed, BYTE InGreen, BYTE InBlue)
{
	ColorRGB Temp;
	Temp.Red = InRed;
	Temp.Green = InGreen;
	Temp.Blue = InBlue;
	return Temp;
}
