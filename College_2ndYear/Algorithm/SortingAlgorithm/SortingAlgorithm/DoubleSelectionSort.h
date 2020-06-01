#pragma once

#include "BaseSort.h"

class DoubleSelectionSort final : public BaseSort
{
public:

	explicit DoubleSelectionSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header);
	virtual ~DoubleSelectionSort() = default;

	void Sort() override;
	void GetMinMaxIndex(int* outMin, int* outMax, int start, int end) const;
	
};

