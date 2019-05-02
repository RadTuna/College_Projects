#pragma once

class Vector
{
public:
	Vector() = delete;
	Vector(int X, int Y);
	virtual ~Vector() = default;

	int GetX() const;
	int GetY() const;

private:
	int X;
	int Y;
};
