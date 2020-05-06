#pragma once

#include "BaseSort.h"

class DoubleSelectionSort final : public BaseSort
{
public:

	explicit DoubleSelectionSort(int* inArray, int arraySize, const std::string& header) : BaseSort(inArray, arraySize, header) {}
	virtual ~DoubleSelectionSort() = default;

	void Sort() override;
	void GetMinMaxIndex(int* outMin, int* outMax, int start, int end) const;
	
};

inline void DoubleSelectionSort::Sort()
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

inline void DoubleSelectionSort::GetMinMaxIndex(int* outMin, int* outMax, int start, int end) const
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
