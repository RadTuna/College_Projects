#include <iostream>

using namespace std;

void RefSwap(int& aVal, int& bVal);
void PtrSwap(int* aVal, int* bVal);

int main()
{
	int numA = 10;
	int numB = 100;

	RefSwap(numA, numB);
	PtrSwap(&numA, &numB);

	return 0;
}

void RefSwap(int& aVal, int& bVal)
{
	int temp = aVal;
	aVal = bVal;
	bVal = temp;
}

void PtrSwap(int* aVal, int* bVal)
{
	int temp = *aVal;
	*aVal = *bVal;
	*bVal = temp;
}
