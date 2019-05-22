#include <iostream>
#include <ctime>

using namespace std;

void InsertSort(int Array[], int Len, bool IsAscending);
void BubbleSort(int Array[] ,int Len, bool IsAscending);
void GenerateArray(int Array[], int Len, int RndSeed);
void PrintArray(int Array[], int Len, const char PreText[]);

int main()
{
	static const int Length = 10;
	int Array[Length];

	srand(static_cast<unsigned int>(time(nullptr)));

	GenerateArray(Array, Length, rand());
	// The argument is false, Sorted in descending
	InsertSort(Array, Length, false);
	PrintArray(Array, Length, "삽입 정렬");

	GenerateArray(Array, Length, rand());
	// The argument is true, Sorted in ascending
	BubbleSort(Array, Length, true);
	PrintArray(Array, Length, "버블 정렬");

	return 0;
}

void InsertSort(int Array[], int Len, bool IsAscending)
{
	for (int i = 1; i < Len; ++i)
	{
		for (int j = i; j >= 1; --j)
		{
			if (Array[j] < Array[j-1] && IsAscending == true)
			{
				int Temp = Array[j];
				Array[j] = Array[j - 1];
				Array[j - 1] = Temp;
			}
			else if (Array[j] > Array[j - 1] && IsAscending == false)
			{
				int Temp = Array[j];
				Array[j] = Array[j - 1];
				Array[j - 1] = Temp;
			}
		}
	}
}

void BubbleSort(int Array[], int Len, bool IsAscending)
{
	for (int i = Len - 1; i >= 0; --i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (Array[j] > Array[j + 1] && IsAscending == true)
			{
				int Temp = Array[j];
				Array[j] = Array[j + 1];
				Array[j + 1] = Temp;
			}
			else if (Array[j] < Array[j + 1] && IsAscending == false)
			{
				int Temp = Array[j];
				Array[j] = Array[j + 1];
				Array[j + 1] = Temp;
			}
		}
	}
}

void GenerateArray(int Array[], int Len, int RndSeed)
{
	srand(RndSeed);
	for (int i = 0; i < Len; ++i)
	{
		Array[i] = rand() % (Len * 10);
	}
}

void PrintArray(int Array[], int Len, const char PreText[])
{
	cout << PreText << endl;
	for (int i = 0; i < Len; ++i)
	{
		cout << i << " : " << Array[i] << endl;
	}
	cout << endl;
}