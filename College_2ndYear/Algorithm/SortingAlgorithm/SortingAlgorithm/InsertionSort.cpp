
#include "InsertionSort.h"

InsertionSort::InsertionSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header)
	: BaseSort(inArray, arraySize, sortDirection, header)
{
}

void InsertionSort::Sort()
{
	mTimeCounter->CaptureStart();
	
	if (mSize < 2)
	{
		return;
	}

	for (int index = 0; index < mSize; ++index)
	{
		int target = 0;
		for (int inner = index - 1; index >= 0; --inner)
		{
			bool bCompareResult = mArray[inner] < mArray[index];
			if (mSortDirection == ESortDirection::Descending)
			{
				bCompareResult = !bCompareResult;
			}
			
			if (bCompareResult)
			{
				target = inner + 1;
				break;
			}
		}

		const int moveSize = index - target;
		const int originData = mArray[index];
		int* srcPtr = &mArray[target];
		int* dstPtr = &mArray[target + 1];
		memmove(dstPtr, srcPtr, moveSize * sizeof(int));
		mArray[target] = originData;
	}

	mTimeCounter->CaptureEnd();
}

