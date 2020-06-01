#pragma once

#include "BaseSort.h"

class BubbleSort final : public BaseSort
{
public:

	explicit BubbleSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header);
	virtual ~BubbleSort() = default;

	void Sort() override;
	
};
