
#include "SelectionSort.h"

SelectionSort::SelectionSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header)
	: BaseSort(inArray, arraySize, sortDirection, header)
{
}

void SelectionSort::Sort()
{
	mTimeCounter->CaptureStart();
	
	for (int outIndex = 0; outIndex < mSize - 1; ++outIndex)
	{
		const int minIndex = GetIndex(outIndex, mSize);
		std::swap(mArray[minIndex], mArray[outIndex]);
	}

	mTimeCounter->CaptureEnd();
}

int SelectionSort::GetIndex(int start, int end) const
{
	int minIndex = start;
	for (int index = start; index < end; ++index)
	{
		bool bCompareResult = mArray[minIndex] > mArray[index];
		if (mSortDirection == ESortDirection::Descending)
		{
			bCompareResult = !bCompareResult;
		}
		
		if (bCompareResult)
		{
			minIndex = index;
		}
	}

	return minIndex;
}
