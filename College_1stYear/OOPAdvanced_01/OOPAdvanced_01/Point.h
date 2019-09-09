#pragma once

class Point
{
public:

	Point() = default;
	~Point() = default;

	void SetPos(float inX, float inY);
	void Show();

private:

	float X;
	float Y;

};