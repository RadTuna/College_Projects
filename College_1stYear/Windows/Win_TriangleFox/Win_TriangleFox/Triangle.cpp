#include "Triangle.h"


void Triangle::Initialize()
{
	VectorU = Point2.Position - Point1.Position;
	VectorV = Point3.Position - Point1.Position;

	DotUU = VectorU.DotProduct(VectorU);
	DotUV = VectorU.DotProduct(VectorV);
	DotVV = VectorV.DotProduct(VectorV);

	WeightDenominator = DotUU * VectorV.DotProduct(VectorV) - DotUV * DotUV;
}

Vector3 Triangle::GetVertexWeight(const Vector2& InPoint)
{
	Vector3 Result;

	VectorW = InPoint - Point1.Position;

	Result.y = (VectorW.DotProduct(VectorU) * DotVV
		- VectorW.DotProduct(VectorV) * DotUV) / WeightDenominator;
	Result.z = (VectorW.DotProduct(VectorV) * DotUU
		- VectorW.DotProduct(VectorU) * DotUV) / WeightDenominator;
	Result.x = 1.0f - Result.y - Result.z;

	return Result;
}