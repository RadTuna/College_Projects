
#include <memory>
#include <iostream>
#include <unordered_map>


#include "RandArrayGenerator.h"
#include "SelectionSort.h"
#include "DoubleSelectionSort.h"
#include "BubbleSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "QuickSort.h"

#define CONSOLE_CRLF std::cout << "\n================================================\n\n"

void ArrayDraw(int* inArray, int arraySize);

enum class ESortType
{
	SelectionSort = 0,
	BubbleSort,
	InsertionSort,
	MergeSort,
	QuickSort
};

int main()
{
	int elementNum = 0;
	std::cout << "요소개수 입력 : ";
	std::cin >> elementNum;

	int inputKey = -1;
	std::cout << "테스트 케이스 정렬 상태 (0-랜덤, 1-오름차순, 2-내림차순) : ";
	std::cin >> inputKey;

	std::vector<int> randArray;
	randArray.resize(elementNum);
	RandNumberGenerator randGenerator(1, elementNum);
	randGenerator.Generate(randArray.data(), elementNum);

	if (inputKey == 1)
	{
		std::sort(randArray.begin(), randArray.end(), std::less<int>());
	}
	else if (inputKey == 2)
	{
		std::sort(randArray.begin(), randArray.end(), std::greater<int>());
	}

	ArrayDraw(randArray.data(), elementNum);

	std::cout << "\n정렬 알고리즘 순서 (0-오름차순, 1-내림차순) : ";
	std::cin >> inputKey;

	ESortDirection sortDirection;
	if (inputKey == 0)
	{
		sortDirection = ESortDirection::Ascending;
	}
	else if (inputKey == 1)
	{
		sortDirection = ESortDirection::Descending;
	}


	std::unordered_map<ESortType, std::unique_ptr<BaseSort>> SortCollection;

	SortCollection[ESortType::SelectionSort] =
		std::make_unique<SelectionSort>(randArray.data(), elementNum, sortDirection, "SelectionSort");
	
	SortCollection[ESortType::BubbleSort] =
		std::make_unique<BubbleSort>(randArray.data(), elementNum, sortDirection, "BubbleSort");
	
	SortCollection[ESortType::InsertionSort] =
		std::make_unique<InsertionSort>(randArray.data(), elementNum, sortDirection, "InsertionSort");
	
	SortCollection[ESortType::MergeSort] =
		std::make_unique<MergeSort>(randArray.data(), elementNum, sortDirection, "MergeSort");

	SortCollection[ESortType::QuickSort] =
		std::make_unique<QuickSort>(randArray.data(), elementNum, sortDirection, "QuickSort");

	CONSOLE_CRLF;

	SortCollection[ESortType::QuickSort]->Sort();
	SortCollection[ESortType::QuickSort]->Draw();
	
	CONSOLE_CRLF;

	SortCollection[ESortType::MergeSort]->Sort();
	SortCollection[ESortType::MergeSort]->Draw();

	CONSOLE_CRLF;

	SortCollection[ESortType::InsertionSort]->Sort();
	SortCollection[ESortType::InsertionSort]->Draw();

	CONSOLE_CRLF;

	SortCollection[ESortType::SelectionSort]->Sort();
	SortCollection[ESortType::SelectionSort]->Draw();

	CONSOLE_CRLF;

	SortCollection[ESortType::BubbleSort]->Sort();
	SortCollection[ESortType::BubbleSort]->Draw();

	CONSOLE_CRLF;

	std::cout << "QuickSortTime : " << SortCollection[ESortType::QuickSort]->GetBenchmark() << "ms" << std::endl;
	std::cout << "MergeSortTime : " << SortCollection[ESortType::MergeSort]->GetBenchmark() << "ms" << std::endl;
	std::cout << "InsertionSortTime : " << SortCollection[ESortType::InsertionSort]->GetBenchmark() << "ms" << std::endl;
	std::cout << "SelectionSortTime : " << SortCollection[ESortType::SelectionSort]->GetBenchmark() << "ms" << std::endl;
	std::cout << "BubbleSortTime : " << SortCollection[ESortType::BubbleSort]->GetBenchmark() << "ms" << std::endl;
	
	return 0;
}

void ArrayDraw(int* inArray, int arraySize)
{
	if (inArray == nullptr)
	{
		return;
	}

	std::cout << "DefaultArray : " << std::endl;
	for (int index = 1; index <= arraySize; ++index)
	{
		std::cout << inArray[index - 1] << " ";
		if (index % 10 == 0)
		{
			std::cout << std::endl;
		}
	}
}
