#pragma once

#include <cassert>
#include <iostream>
#include <string>

class BaseSort
{
public:
	
	explicit BaseSort(int* inArray, int arraySize, const std::string& header)
	{
		assert(inArray != nullptr);
		mArray = new int[arraySize];
		mSize = arraySize;

		memcpy(mArray, inArray, arraySize * sizeof(inArray));

		mHeader = header;
	}
	virtual ~BaseSort()
	{
		if (mArray != nullptr)
		{
			delete[] mArray;
			mArray = nullptr;
			mSize = 0;
		}
	}

	virtual void Sort() = 0;
	void Draw();

protected:

	int* mArray = nullptr;
	int mSize = 0;
	std::string mHeader;
	
};

inline void BaseSort::Draw()
{
	std::cout << mHeader << " : " << std::endl;
	for (int index = 1; index <= mSize; ++index)
	{
		std::cout << mArray[index - 1] << " ";
		if (index % 10 == 0)
		{
			std::cout << std::endl;
		}
	}
}
