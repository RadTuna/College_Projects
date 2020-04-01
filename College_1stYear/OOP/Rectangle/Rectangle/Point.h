#pragma once

class Point
{
public:

	Point();
	Point(float InX, float InY);
	~Point();

	void SetX(float InX);
	void SetY(float InY);

private:

	float X;
	float Y;

public:

	inline float GetX() const { return X; }
	inline float GetY() const { return Y; }

};
