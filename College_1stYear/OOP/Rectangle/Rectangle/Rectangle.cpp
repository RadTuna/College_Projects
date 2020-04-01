#include "Rectangle.h"
#include <cmath>

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(const Point& InFirst, const Point& InSecond)
	: FirstPoint(InFirst),
	SecondPoint(InSecond)
{
}

Rectangle::Rectangle(float InFirstX, float InFirstY, float InSecondX, float InSecondY)
{
	FirstPoint.SetX(InFirstX);
	FirstPoint.SetY(InFirstY);
	SecondPoint.SetX(InSecondX);
	SecondPoint.SetY(InSecondY);
}

Rectangle::~Rectangle()
{
}

void Rectangle::SetFirstPoint(const Point& InPoint)
{
	FirstPoint = InPoint;
}

void Rectangle::SetSecondPoint(const Point& InPoint)
{
	SecondPoint = InPoint;
}

float Rectangle::GetRectArea() const
{
	float Width = std::abs(FirstPoint.GetX() - SecondPoint.GetX());
	float Height = std::abs(FirstPoint.GetY() - SecondPoint.GetY());

	return Width * Height;
}
