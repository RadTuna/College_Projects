
#include "BubbleSort.h"

BubbleSort::BubbleSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header)
    : BaseSort(inArray, arraySize, sortDirection, header)
{
}

void BubbleSort::Sort()
{
	mTimeCounter->CaptureStart();
	
	bool bIsSorted = false;
	for (int outIndex = mSize - 1; outIndex >= 0 && !bIsSorted; --outIndex)
	{
		bIsSorted = true;
		for (int index = 0; index < outIndex; ++index)
		{
			bool bCompareResult = mArray[index] > mArray[index + 1];
			if (mSortDirection == ESortDirection::Descending)
			{
				bCompareResult = !bCompareResult;
			}
			
			if (bCompareResult)
			{
				std::swap(mArray[index], mArray[index + 1]);
				bIsSorted = false;
			}
		}
	}

	mTimeCounter->CaptureEnd();
}

