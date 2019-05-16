#include <iostream>
#include "Fibonacci.h"

using namespace std;

int Fibonacci(int Num)
{
	if (Num == 0)
	{
		return 0;
	}
	if (Num == 1)
	{
		return 1;
	}

	return (Fibonacci(Num - 1) + Fibonacci(Num - 2));
}