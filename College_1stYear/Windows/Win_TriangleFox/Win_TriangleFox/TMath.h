#pragma once

#include "CommonHeader.h"

struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float InX, float InY);
	Vector2 operator-(const Vector2& Other) const;
	float DotProduct(const Vector2& Other) const;
};

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float InX, float InY, float InZ);
};

struct ColorRGB
{
	BYTE Red;
	BYTE Green;
	BYTE Blue;

	ColorRGB();
	ColorRGB(BYTE InRed, BYTE InGreen, BYTE InBlue);
	ColorRGB operator*(const float& Other) const;
	ColorRGB operator+(const ColorRGB& Other) const;
};

class TMath
{
public:

	template <typename T>
	static T Clamp(T targetVal, T minVal, T maxVal);

	static Vector2 Vector2Set(float InX, float InY);
	static Vector3 Vector3Set(float InX, float InY, float InZ);
	static ColorRGB ColorSet(BYTE InRed, BYTE InGreen, BYTE InBlue);

};

template<typename T>
T TMath::Clamp(T targetVal, T minVal, T maxVal)
{
	if (targetVal > maxVal)
	{
		return maxVal;
	}
	else if (targetVal < minVal)
	{
		return minVal;
	}

	return targetVal;
}

