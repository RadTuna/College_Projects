#pragma once

template<typename T>
class MyArray {

public:

	MyArray()
	{
		ArraySize = 0;
	}

	bool Add(T data)
	{
		if (ArraySize == MAX_SIZE)
		{
			return false;
		}

		Array[ArraySize++] = data;

		return true;
	}

	int GetArraySize() const
	{
		return ArraySize;
	}

	T GetArrayOrNull(int Index) const
	{
		if (Index < 0 || Index > MAX_SIZE - 1)
		{
			return NULL;
		}
		return Array[Index];
	}

private:

	enum { MAX_SIZE = 3 };
	int ArraySize;
	T Array[MAX_SIZE];
};
