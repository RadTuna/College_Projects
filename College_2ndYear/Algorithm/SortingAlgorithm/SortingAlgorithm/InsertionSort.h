#pragma once

#include <memory>

#include "BaseSort.h"

class InsertionSort final : public BaseSort
{
public:

	explicit InsertionSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header);
	virtual ~InsertionSort() = default;

	void Sort() override;
	
};
