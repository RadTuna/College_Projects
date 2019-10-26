#pragma once

struct Point
{
	int X;
	int Y;

	Point() = default;
	Point(int InX, int InY) : X(InX), Y(InY) {}
};

class List
{
public:

	List(int Reserve);
	~List();

	bool Insert(int Index, const Point& InData);
	bool Remove(int Index);
	const Point* Retrieve(int Index) const;
	bool IsEmpty() const;
	bool IsFull() const;
	void Clear(Point* InArr);

private:

	Point* Array;
	int Lenght;
	int Capacity;

public:

	__forceinline int GetLenght() const { return Lenght; }
	__forceinline int GetCapacity() const { return Capacity; }

};
