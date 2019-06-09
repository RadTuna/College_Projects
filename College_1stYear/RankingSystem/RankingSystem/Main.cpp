#include <iostream>
#include <ctime>

using namespace std;

void Swap(int& LNum, int& RNum);
void QuickSort(int* InArr, int Left, int Right);
void ArrGenerator(int* InArr, int Len);
void PrintArr(int* InArr, int Len, int Header); 
void SortedAppend(int* InArr, int Len, int InsertNum); // Array의 모든요소보다 값이 작으면 삽입안됨.
int MinIndex(int* InArr, int Len);

int main()
{
	static const int Len = 5;
	int Array[5];

	srand(static_cast<unsigned int>(time(nullptr)));

	ArrGenerator(Array, Len);
	for (int i = 0; i < 8; ++i)
	{
		int rnd = rand() % 100;
		SortedAppend(Array, Len, rnd);
		PrintArr(Array, Len, rnd);
		cout << endl;
	}
	return 0;
}

void Swap(int& LNum, int& RNum)
{
	if (LNum == RNum)
	{
		return;
	}

	int Temp = LNum;
	LNum = RNum;
	RNum = Temp;
}

void QuickSort(int* InArr, int Left, int Right)
{
	if (Right < Left)
	{
		return;
	}

	int tLeft = Left;
	int Pivot = InArr[(Left + Right) / 2];
	Swap(InArr[(Left + Right) / 2], InArr[Right]);

	for (int i = Left; i < Right; ++i)
	{
		if (InArr[i] > Pivot)
		{
			Swap(InArr[tLeft], InArr[i]);
			tLeft += 1;
		}
	}
	Swap(InArr[tLeft], InArr[Right]);

	QuickSort(InArr, Left, tLeft - 1);
	QuickSort(InArr, tLeft + 1, Right);
}

void ArrGenerator(int* InArr, int Len)
{
	for (int i = 0; i < Len; ++i)
	{
		InArr[i] = 0;
	}
}

void PrintArr(int* InArr, int Len, int Header)
{
	cout << "[" << Header << "]" << " 순위 : ";
	for (int i = 0; i < Len; ++i)
	{
		cout << InArr[i] << " ";
	}
}

void SortedAppend(int* InArr, int Len, int InsertNum)
{
	int Index = MinIndex(InArr, Len);
	if (InArr[Index] < InsertNum)
	{
		InArr[Index] = InsertNum;
		QuickSort(InArr, 0, Len);
	}
}

int MinIndex(int* InArr, int Len)
{
	int MinNum = numeric_limits<int>::max();
	int MinIndex = -1;

	for (int i = 0; i < Len; ++i)
	{
		if (MinNum > InArr[i])
		{
			MinNum = InArr[i];
			MinIndex = i;
		}
	}
	
	return MinIndex;
}
