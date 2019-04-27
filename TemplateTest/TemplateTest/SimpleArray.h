#pragma once

template<typename T>
class SimpleArray
{
public:
	SimpleArray();

	bool Add(T data);
	T GetArray(int Index) const;

private:
	enum { MAX = 3 };
	int Size;
	T Array[MAX];
};

template<typename T>
SimpleArray<T>::SimpleArray()
{
	Size = 0;
}

template<typename T>
bool SimpleArray<T>::Add(T data)
{
	if (Size == MAX)
	{
		return false;
	}

	Array[Size++] = data;
	return true;
}

template<typename T>
T SimpleArray<T>::GetArray(int Index) const
{
	if (Index < 0 || Index > MAX - 1)
	{
		return 0;
	}

	return Array[Index];
}