#include "List.h"

List::List(int Reserve)
	: Capacity(Reserve),
	Lenght(0)
{
	Array = new Point[Capacity];

	Clear(Array);
}

List::~List()
{
	if (Array != nullptr)
	{
		delete[] Array;
		Array = nullptr;
	}
}

bool List::Insert(int Index, const Point& InData)
{
	if (Index < 0 || Index >= Capacity)
	{
		return false;
	}
	if (Index > Lenght)
	{
		return false;
	}
	if (IsFull() == true) // == Full
	{
		Point* NewArray = new Point[Capacity * 2];
		Clear(NewArray);

		for (int i = 0; i < Capacity; ++i)
		{
			NewArray[i] = Array[i];
		}
		Capacity *= 2;
		delete[] Array;
		Array = NewArray;
	}

	for (int i = Lenght; i > Index; --i)
	{
		if (i >= Capacity)
		{
			break;
		}

		Array[i] = Array[i - 1];
	}

	Array[Index] = InData;
	Lenght++;

	return true;
}

bool List::Remove(int Index)
{
	if (Index < 0 || Index >= Capacity)
	{
		return false;
	}
	if (IsEmpty() == true) // == Empty
	{
		return false;
	}
	if (Index > Lenght)
	{
		return false;
	}

	for (int i = Index; i < Lenght; ++i)
	{
		if (i >= Capacity - 1)
		{
			break;
		}

		Array[i] = Array[i + 1];
	}
	Array[Lenght - 1] = { -1, -1 };
	Lenght--;

	return true;
}

bool List::IsEmpty() const
{
	return Lenght <= 0 ? true : false;
}

bool List::IsFull() const
{
	return Lenght >= Capacity ? true : false;
}

const Point* List::Retrieve(int Index) const
{
	if (Index > Lenght)
	{
		return nullptr;
	}

	return &Array[Index];
}

void List::Clear(Point* InArr)
{
	for (int i = 0; i < Capacity; ++i)
	{
		InArr[i] = { -1, -1 };
	}
}
