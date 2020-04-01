#include <iostream>
#include <iomanip>

using namespace std;

// 3 * 3 Matrix, I need a double pointer
void MatrixMul(int MulMatrix[3][3], int AMatrix[3][3], int BMatrix[3][3]);
void InitMatrix(int Matrix[3][3], bool UseInitValue);
void PrintMatrix(int Matrix[3][3], const char PreText[]);

int main()
{
	int MulMatrix[3][3];
	int AMatrix[3][3];
	int BMatrix[3][3];

	InitMatrix(MulMatrix, false);
	InitMatrix(AMatrix, true);
	InitMatrix(BMatrix, true);

	PrintMatrix(AMatrix, "AMatrix");
	PrintMatrix(BMatrix, "BMatrix");

	MatrixMul(MulMatrix, AMatrix, BMatrix);

	PrintMatrix(MulMatrix, "AMatrix * BMatrix");

	return 0;
}

// 3 * 3 Matrix, I need a double pointer
void MatrixMul(int MulMatrix[3][3], int AMatrix[3][3], int BMatrix[3][3])
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				MulMatrix[i][j] += AMatrix[i][k] * BMatrix[k][j];
			}
		}
	}
}

void InitMatrix(int Matrix[3][3], bool UseInitValue)
{
	if (UseInitValue == true)
	{
		int Value = 2;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j, Value += 2)
			{
				Matrix[j][i] = Value;
			}
		}
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				Matrix[i][j] = 0;
			}
		}
	}
}

void PrintMatrix(int Matrix[3][3], const char PreText[])
{
	cout << PreText << endl;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cout << setw(3) << Matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}