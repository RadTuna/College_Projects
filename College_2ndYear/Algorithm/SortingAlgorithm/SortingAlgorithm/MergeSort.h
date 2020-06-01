#pragma once

#include "BaseSort.h"

class MergeSort final : public BaseSort
{
public:

	explicit MergeSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header);
	virtual ~MergeSort();

	void Sort() override;
	void MergeInternalRecursive(int start, int end);

private:

	int* mBuffer;
	int mBufferSize;
	
};
