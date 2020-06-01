#pragma once

#include <utility>

#include "BaseSort.h"

class SelectionSort final : public BaseSort
{
public:

	explicit SelectionSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header);
	virtual ~SelectionSort() = default;

	void Sort() override;
	int GetIndex(int start, int end) const;

};

