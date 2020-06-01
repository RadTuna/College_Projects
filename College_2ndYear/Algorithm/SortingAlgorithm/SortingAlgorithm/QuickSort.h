#pragma once

#include "BaseSort.h"


class QuickSort final : public BaseSort
{
public:

	explicit QuickSort (int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header);
	virtual ~QuickSort() = default;

	void Sort() override;
	void SortInternalRecursive(int left, int right) const;
	int PartitionSort(int left, int right) const;

};

