
#include "DoubleSelectionSort.h"

DoubleSelectionSort::DoubleSelectionSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header)
	: BaseSort(inArray, arraySize, sortDirection, header)
{
}

void DoubleSelectionSort::Sort()
{
	for (int startIndex = 0, endIndex = mSize - 1; endIndex > startIndex; ++startIndex, --endIndex)
	{
		int minIndex = -1;
		int maxIndex = -1;
		GetMinMaxIndex(&minIndex, &maxIndex, startIndex, endIndex + 1);

		if (minIndex == endIndex && maxIndex == startIndex)
		{
			std::swap(mArray[minIndex], mArray[maxIndex]);
		}
		else if (minIndex == endIndex)
		{
			std::swap(mArray[minIndex], mArray[startIndex]);
			std::swap(mArray[maxIndex], mArray[endIndex]);
		}
		else
		{
			std::swap(mArray[maxIndex], mArray[endIndex]);
			std::swap(mArray[minIndex], mArray[startIndex]);
		}
	}
}

void DoubleSelectionSort::GetMinMaxIndex(int* outMin, int* outMax, int start, int end) const
{
	*outMin = start;
	*outMax = start;
	for (int index = start; index < end; ++index)
	{
		if (mArray[*outMin] > mArray[index])
		{
			*outMin = index;
		}

		if (mArray[*outMax] < mArray[index])
		{
			*outMax = index;
		}
	}
}
