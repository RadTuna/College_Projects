
#include <map>
#include <memory>

#include "SelectionSort.h"
#include "DoubleSelectionSort.h"
#include "BubbleSort.h"
#include "RandArrayGenerator.h"

#define CONSOLE_CRLF std::cout << "\n================================================\n\n"

void ArrayDraw(int* inArray, int arraySize);

enum class ESortType
{
	SelectionSort = 0,
	DoubleSelectionSort,
	BubbleSort
};

int main()
{
	enum { RAND_ARRAY_SIZE = 100 };
	
	int* randArray = new int[RAND_ARRAY_SIZE];
	RandNumberGenerator randGenerator(1, 100);
	randGenerator.Generate(randArray, RAND_ARRAY_SIZE);

	ArrayDraw(randArray, RAND_ARRAY_SIZE);
	
	std::map<ESortType, std::unique_ptr<BaseSort>> SortCollection;
	SortCollection[ESortType::SelectionSort] = 
		std::make_unique<SelectionSort>(randArray, RAND_ARRAY_SIZE, "SelectionSort");
	SortCollection[ESortType::DoubleSelectionSort] = 
		std::make_unique<DoubleSelectionSort>(randArray, RAND_ARRAY_SIZE, "DoubleSelectionSort");
	SortCollection[ESortType::BubbleSort] = 
		std::make_unique<BubbleSort>(randArray, RAND_ARRAY_SIZE, "BubbleSort");
	
	CONSOLE_CRLF;

	SortCollection[ESortType::SelectionSort]->Sort();
	SortCollection[ESortType::SelectionSort]->Draw();

	CONSOLE_CRLF;
	
	SortCollection[ESortType::DoubleSelectionSort]->Sort();
	SortCollection[ESortType::DoubleSelectionSort]->Draw();

	CONSOLE_CRLF;
	
	SortCollection[ESortType::BubbleSort]->Sort();
	SortCollection[ESortType::BubbleSort]->Draw();

	delete[] randArray;
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
