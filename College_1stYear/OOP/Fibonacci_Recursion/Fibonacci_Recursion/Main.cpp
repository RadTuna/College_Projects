#include <iostream>
#include "Fibonacci.h"

using namespace std;

int main()
{
	int Num = 0;

	cout << "N��ŭ�� �Ǻ���ġ ���� ���: ";
	cin >> Num;

	for (int i = 0; i < Num; ++i)
	{
		cout << "[" << i << "] " << Fibonacci(i) << endl;
	}

	return 0;
}