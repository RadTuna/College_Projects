#pragma once

#include <utility>

#include "BaseSort.h"

class SelectionSort final : public BaseSort
{
public:

	explicit SelectionSort(int* inArray, int arraySize, const std::string& header) : BaseSort(inArray, arraySize, header) {}
	virtual ~SelectionSort() = default;

	void Sort() override;
	int GetMinIndex(int start, int end) const;

};

inline void SelectionSort::Sort()
{
	for (int outIndex = 0; outIndex < mSize - 1; ++outIndex)
	{
		const int minIndex = GetMinIndex(outIndex, mSize);
		std::swap(mArray[minIndex], mArray[outIndex]);
	}
}

inline int SelectionSort::GetMinIndex(int start, int end) const
{
	int minIndex = start;
	for (int index = start; index < end; ++index)
	{
		if (mArray[minIndex] > mArray[index])
		{
			minIndex = index;
		}
	}

	return minIndex;
}
