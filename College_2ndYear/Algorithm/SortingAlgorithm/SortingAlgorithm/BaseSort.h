#pragma once

#include <string>
#include <memory>

#include "TimeCounter.h"

enum class ESortDirection
{
	Ascending,
	Descending
};

class BaseSort
{
public:
	
	explicit BaseSort(int* inArray, unsigned int arraySize, ESortDirection sortDirection, const std::string& header);
	virtual ~BaseSort();

	virtual void Sort() = 0;
	void Draw();
	float GetBenchmark();
	int* GetData() const { return mArray; }

protected:

	int* mArray = nullptr;
	int mSize = 0;
	std::string mHeader;
	ESortDirection mSortDirection;

	std::unique_ptr<TimeCounter> mTimeCounter;
	
};

