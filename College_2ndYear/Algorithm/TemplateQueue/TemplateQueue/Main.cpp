
#include <iostream>
#include <vector>
#include <algorithm>

#include "Queue.h"


int main()
{
	Queue<int> testQueue;

	testQueue.Enqueue(5);
	testQueue.Enqueue(8);
	testQueue.Enqueue(9);
	testQueue.Enqueue(14);

	testQueue.Draw();

	int outData = 0;
	testQueue.Dequeue(&outData);
	std::cout << "out : " << outData << std::endl;

	testQueue.Draw();

	std::vector<int> testVector;
	testVector.emplace_back(10);
	testVector.emplace_back(11);
	testVector.emplace_back(12);
	testVector.emplace_back(13);

	int baseNum = 12;
	std::vector<int>::iterator iter = std::find_if(testVector.begin(), testVector.end(), [baseNum](int element) -> bool
	{
		return element == baseNum;
	});

	std::cout << "iter : " << *iter << std::endl;

	return 0;
}
