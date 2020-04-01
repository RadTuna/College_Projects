#include <iostream>
#include "Fibonacci.h"

using namespace std;

int main()
{
	int Num = 0;

	cout << "N만큼의 피보나치 수열 출력: ";
	cin >> Num;

	for (int i = 0; i < Num; ++i)
	{
		cout << "[" << i << "] " << Fibonacci(i) << endl;
	}

	return 0;
}