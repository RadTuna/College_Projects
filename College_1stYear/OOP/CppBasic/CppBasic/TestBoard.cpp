#include <iostream>

using namespace std;

int main()
{
	int i = 0;
	while (3 > i)
	{
		i++;
		if (3 == i)
		{
			continue;
		}
	}
}