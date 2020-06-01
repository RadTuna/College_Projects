
#include "QuickSort.h"


QuickSort::QuickSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header)
	: BaseSort(inArray, arraySize, sortDirection, header)
{
}

void QuickSort::Sort()
{
	mTimeCounter->CaptureStart();
	
	SortInternalRecursive(0, mSize);

	mTimeCounter->CaptureEnd();
}

// [left, right)
void QuickSort::SortInternalRecursive(int left, int right) const
{
	if (right <= left)
	{
		return;
	}

	const int pivotIndex = PartitionSort(left, right);

	SortInternalRecursive(left, pivotIndex);
	SortInternalRecursive(pivotIndex + 1, right);
}

// [left, right)
int QuickSort::PartitionSort(int left, int right) const
{
	const int pivotItem = mArray[left];
	int pivotIndex = left;

	for (int i = left + 1; i < right; ++i)
	{
		bool bCompareResult = pivotItem > mArray[i];
		if (mSortDirection == ESortDirection::Descending)
		{
			bCompareResult = !bCompareResult;
		}
		
		if (bCompareResult)
		{
			++pivotIndex;
			std::swap(mArray[pivotIndex], mArray[i]);
		}
	}

	std::swap(mArray[left], mArray[pivotIndex]);
	return pivotIndex;
}
