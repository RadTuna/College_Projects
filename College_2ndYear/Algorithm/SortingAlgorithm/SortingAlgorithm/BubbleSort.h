#pragma once

#include "BaseSort.h"

class BubbleSort final : public BaseSort
{
public:

	explicit BubbleSort(int* inArray, int arraySize, const std::string& header) : BaseSort(inArray, arraySize, header) {}
	virtual ~BubbleSort() = default;

	void Sort() override;
	
};

inline void BubbleSort::Sort()
{
	bool bIsSorted = false;
	for (int outIndex = mSize - 1; outIndex >= 0 && !bIsSorted; --outIndex)
	{
		bIsSorted = true;
		for (int index = 0; index < outIndex; ++index)
		{
			if (mArray[index] > mArray[index + 1])
			{
				std::swap(mArray[index], mArray[index + 1]);
				bIsSorted = false;
			}
		}
	}
}

