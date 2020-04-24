
#include<iostream>
#include<string>

#include "Graph.h"


int main()
{
	enum { MAX_NODE = 6 };

	char nodeName[MAX_NODE] = { 'A', 'B', 'C', 'D', 'E', 'F' };
	int nodeConnections[MAX_NODE][MAX_NODE] = {
		{ 0,        50,        10,      1000,    45,       1000 },
		{ 1000,    0,        15,       1000,    10,       1000 },
		{ 20,       1000,    0,        15,       1000,    1000 },
		{ 1000,    20,       1000,    0,        35,       10    },
		{ 1000,    1000,    1000,    30,       0,        1000 },
		{ 1000,    1000,    1000,    3,        1000,    0     }
	};
	
	Graph<char> graph(nodeName, &nodeConnections[0][0], MAX_NODE);
	
	int* parentNode = new int[MAX_NODE];
	memset(parentNode, 0, sizeof(int) * MAX_NODE);

	
	std::cout << "0 : A\n1 : B\n2 : C\n3 : D\n4 : E\n5 : F" << std::endl;
	
	std::cout << "시작노드 입력 : ";
	int start = -1;
	std::cin >> start;

	std::cout << "도착노드 입력 : ";
	int end = -1;
	std::cin >> end;

	int* outArray = nullptr;
	int arraySize = 0;
	const bool bIsFind = graph.FindPath(&outArray, &arraySize, start, end);
	
	if (bIsFind)
	{
		std::cout << "노드를 찾았습니다." << std::endl;

		std::cout << "경로 : ";
		for (int index = 0; index < arraySize; ++index)
		{
			if (index == arraySize - 1)
			{
				std::cout << nodeName[outArray[index]];
				break;
			}
			std::cout << nodeName[outArray[index]] << " -> ";
		}

		std::cout << std::endl;
	}
	else
	{
		std::cout << "노드를 찾지 못했습니다." << std::endl;
	}

	
	delete[] outArray;
	outArray = nullptr;

	return 0;
}
