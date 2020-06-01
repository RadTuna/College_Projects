
#include "MergeSort.h"

MergeSort::MergeSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header)
	: BaseSort(inArray, arraySize, sortDirection, header)
{
	mBuffer = new int[arraySize];
	mBufferSize = arraySize;
}

MergeSort::~MergeSort()
{
	if (mBuffer != nullptr)
	{
		delete[] mBuffer;
		mBuffer = nullptr;
	}
}

void MergeSort::Sort()
{
	mTimeCounter->CaptureStart();
	
	MergeInternalRecursive(0, mSize);

	mTimeCounter->CaptureEnd();
}

void MergeSort::MergeInternalRecursive(int start, int end)
{
	if (start >= end - 1)
	{
		return;
	}

	const int mid = (start + end) / 2;
	MergeInternalRecursive(start, mid);
	MergeInternalRecursive(mid, end);

	// Merge
	int leftIndex = start;
	int rightIndex = mid;
	const int arraySize = end - start;
	for (int index = 0; index < arraySize; ++index)
	{
		if (leftIndex >= mid)
		{
			mBuffer[index] = mArray[rightIndex];
			++rightIndex;
			continue;
		}

		if (rightIndex >= end)
		{
			mBuffer[index] = mArray[leftIndex];
			++leftIndex;
			continue;
		}

		bool bCompareResult = mArray[leftIndex] < mArray[rightIndex];
		if (mSortDirection == ESortDirection::Descending)
		{
			bCompareResult = !bCompareResult;
		}
		
		if (bCompareResult)
		{
			mBuffer[index] = mArray[leftIndex];
			++leftIndex;
		}
		else
		{
			mBuffer[index] = mArray[rightIndex];
			++rightIndex;
		}
	}
	memcpy(&mArray[start], mBuffer, arraySize * sizeof(int));
}

