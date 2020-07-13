
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>

#define INDEX_NONE INT_MAX
#define USE_RECURSIVE false;
using uint = unsigned int;


template<typename Type, typename Pred>
uint BinarySearchRecursive(Type* outArray, const Type& target, uint low, uint high, Pred comparer)
{
	if (low > high)
	{
		return INDEX_NONE;
	}

	uint midIndex = (low + high) / 2;
	if (comparer(target, outArray[midIndex]) == 0)
	{
		return midIndex;
	}
	else if (comparer(target, outArray[midIndex]) < 0)
	{
		return BinarySearchRecursive(outArray, target, low, midIndex - 1, comparer);
	}
	else
	{
		return BinarySearchRecursive(outArray, target, midIndex + 1, high, comparer);
	}
}

template<typename Type, typename Pred>
uint BinarySearch(Type* outArray, uint size, const Type& target, Pred comparer)
{
	uint lowIndex = 0;
	uint highIndex = size - 1;
	while (lowIndex <= highIndex)
	{
		uint midIndex = (lowIndex + highIndex) / 2;
		
		if (comparer(target, outArray[midIndex]) == 0)
		{
			return midIndex;
		}
		else if (comparer(target, outArray[midIndex]) < 0)
		{
			highIndex = midIndex - 1;
		}
		else
		{
			lowIndex = midIndex + 1;
		}
	}

	return INDEX_NONE;
}

template<typename Type>
void PrintArray(const Type* inArray, uint size)
{
	for (uint i = 1; i <= size; ++i)
	{
		std::cout << inArray[i - 1] << " ";
		if (i % 10 == 0)
		{
			std::cout << std::endl;
		}
	}
}

int main()
{
	enum { TEST_CASE_SIZE = 100 };

	std::random_device rngDev;
	std::uniform_int_distribution<int> intDist(0, TEST_CASE_SIZE);

	int testArray[TEST_CASE_SIZE];

	for (uint i = 0; i < TEST_CASE_SIZE; ++i)
	{
		testArray[i] = intDist(rngDev);
	}

	auto comparer = [](int valueA, int valueB) -> int
	{
		if (valueA == valueB)
		{
			return 0;
		}
		else if (valueA < valueB)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	};

	std::sort(testArray, testArray + TEST_CASE_SIZE);
	PrintArray(testArray, TEST_CASE_SIZE);

	int targetElement = 0;
	std::cout << "\n탐색할 요소 입력 :";
	std::cin >> targetElement;

#if USE_RECURSIVE
	uint findIndex = BinarySearchRecursive(testArray, targetElement, 0, TEST_CASE_SIZE, comparer);
#else
	uint findIndex = BinarySearch(testArray, TEST_CASE_SIZE, targetElement, comparer);
#endif

	
	if (findIndex == INDEX_NONE)
	{
		std::cout << "do not find element." << std::endl;
	}
	else
	{
		std::cout << "TargetElement : " << targetElement << std::endl;
		std::cout << "Index : " << findIndex << std::endl;
		std::cout << "Element : " << testArray[findIndex] << std::endl;
	}

	return 0;
}
