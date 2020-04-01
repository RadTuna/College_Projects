#pragma once

#include "Point.h"

class Rectangle
{
public:

	Rectangle();
	Rectangle(const Point& InFirst, const Point& InSecond);
	Rectangle(float InFirstX, float InFirstY, float InSecondX, float InSecondY);
	~Rectangle();

	void SetFirstPoint(const Point& InPoint);
	void SetSecondPoint(const Point& InPoint);

	float GetRectArea() const;

private:

	Point FirstPoint;
	Point SecondPoint;

};