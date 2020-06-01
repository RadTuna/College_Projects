
#include <cassert>
#include <iostream>

#include "BaseSort.h"

BaseSort::BaseSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header)
{
	assert(inArray != nullptr);
	mArray = new int[arraySize];
	mSize = arraySize;

	const unsigned int copySize = arraySize * sizeof(int);
	memcpy(mArray, inArray, copySize);

	mHeader = header;

	mSortDirection = sortDirection;
	mTimeCounter = std::make_unique<TimeCounter>();
}

BaseSort::~BaseSort()
{
	if (mArray != nullptr)
	{
		delete[] mArray;
		mArray = nullptr;
		mSize = 0;
	}
}

void BaseSort::Draw()
{
	std::cout << mHeader << " / SortTime : " << GetBenchmark() << "ms" << std::endl;
	for (int index = 1; index <= mSize; ++index)
	{
		std::cout << mArray[index - 1] << " ";
		if (index % 10 == 0)
		{
			std::cout << std::endl;
		}
	}
}

float BaseSort::GetBenchmark()
{
	return mTimeCounter->GetDeltaTime();
}
