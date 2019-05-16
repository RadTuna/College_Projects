#include <iostream>
#include "TowerOfHanoi.h"

using namespace std;

// Start:StartPoint, Des:EndPoint, Mid:MidPoint
int TowerOfHanoi(Hanoi Start, Hanoi Dest, Hanoi Mid, int Count)
{
	static int TotalMove = 0;

	if (Count == 1)
	{
		cout << static_cast<int>(Start) << "에 있는 블럭을 " << static_cast<int>(Dest) << "에 이동." << endl;
		TotalMove++;
		return 0;
	}

	TowerOfHanoi(Start, Mid, Dest, Count - 1);

	cout << static_cast<int>(Start) << "에 있는 블럭을 " << static_cast<int>(Dest) << "에 이동." << endl;
	TotalMove++;

	TowerOfHanoi(Mid, Dest, Start, Count - 1);

	return TotalMove;
}