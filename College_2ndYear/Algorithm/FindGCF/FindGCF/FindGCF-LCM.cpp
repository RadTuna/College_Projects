#include <iostream>

int GetMax(int numA, int numB);
int GetMin(int numA, int numB);

int FindGCFRecursive(int numA, int numB);
int FindLCMRecursive(int numA, int numB);

int FindGCF(int numA, int numB);
int FindLCM(int numA, int numB);

int main()
{
	std::cout << "두 수를 입력하세요." << std::endl;

	int numA = 0;
	int numB = 0;

	std::cin >> numA >> numB;

	int numGCF = FindGCF(numA, numB);
	int numLCM = FindLCM(numA, numB);
	std::cout << "최대공약수(반복) : " << numGCF << std::endl;
	std::cout << "최소공배수(반복) : " << numLCM << std::endl;
	
	numGCF = FindGCFRecursive(numA, numB);
	numLCM = FindLCMRecursive(numA, numB);
	std::cout << "최대공약수(재귀) : " << numGCF << std::endl;
	std::cout << "최소공배수(재귀) : " << numLCM << std::endl;

	return 0;
}

int GetMax(int numA, int numB)
{
	return numA > numB ? numA : numB;
}

int GetMin(int numA, int numB)
{
	return numA < numB ? numA : numB;
}

int FindGCFRecursive(int numA, int numB)
{
	if (numA == numB)
	{
		return numA;
	}

	if (numA > numB)
	{
		return FindGCFRecursive(numA - numB, numB);
	}
	else
	{
		return FindGCFRecursive(numA, numB - numA);
	}
}

int FindLCMRecursive(int numA, int numB)
{
	const int numGCF = FindGCFRecursive(numA, numB);

	return (numA * numB) / numGCF;
}

int FindGCF(int numA, int numB)
{
	for (;numA != numB;)
	{
		if (numA > numB)
		{
			numA -= numB;
		}
		else
		{
			numB -= numA;
		}
	}

	return numA;
}

int FindLCM(int numA, int numB)
{
	const int numGCF = FindGCF(numA, numB);

	return (numA * numB) / numGCF;
}

