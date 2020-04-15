#include <iostream>

int FindFibonacciRecursive(int num);
int FindFibonacci(int num);

int main()
{
	std::cout << "n��° �Ǻ���ġ ���� ����մϴ�. n�� �Է��ϼ���." << std::endl;

	int num = 0;

	std::cin >> num;

	int numFibo = FindFibonacci(num);
	std::cout << "�Ǻ���ġ ���� ��(�ݺ�) : " << numFibo << std::endl;
	
	numFibo = FindFibonacciRecursive(num);
	std::cout << "�Ǻ���ġ ���� ��(���) : " << numFibo << std::endl;

	return 0;
}

int FindFibonacciRecursive(int num)
{
	if (num == 0)
	{
		return 0;
	}
	if (num == 1)
	{
		return 1;
	}

	return FindFibonacciRecursive(num - 1) + FindFibonacciRecursive(num - 2);
}

int FindFibonacci(int num)
{
	if (num == 0)
	{
		return 0;
	}
	
	int resultNum = 1;
	for (int prevNum = 0, count = 0; count < num - 1; ++count)
	{
		resultNum += prevNum;
		prevNum = resultNum - prevNum;
	}

	return resultNum;
}

