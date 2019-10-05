#pragma once

#include "CommonHeader.h"
#include "TMath.h"

struct Vertex
{

	Vector2 Position;
	ColorRGB VertexColor;

};

struct Triangle
{

	Vertex Point1;
	Vertex Point2;
	Vertex Point3;

public:

	void Initialize();

	// 삼각형 내부포인트의 버텍스 별 가중치를 계산.
	Vector3 GetVertexWeight(const Vector2& InPoint);

private:

	Vector2 VectorU;
	Vector2 VectorV;
	Vector2 VectorW;
	float WeightDenominator;
	float DotUV;
	float DotUU;
	float DotVV;

};
