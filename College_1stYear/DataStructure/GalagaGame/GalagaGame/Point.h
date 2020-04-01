#pragma once
class Point final
{
public:

	explicit Point();
	explicit Point(int InX, int InY);
	~Point() = default;

	void SetPoint(int InX, int InY);

private:

	int X;
	int Y;

public:

	__forceinline int GetPointX() const { return X; }
	__forceinline int GetPointY() const { return Y; }

};

