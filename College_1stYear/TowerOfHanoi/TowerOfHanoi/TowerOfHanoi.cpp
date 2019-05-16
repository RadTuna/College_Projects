#include <iostream>
#include "TowerOfHanoi.h"

using namespace std;

// Start:StartPoint, Des:EndPoint, Mid:MidPoint
int TowerOfHanoi(Hanoi Start, Hanoi Dest, Hanoi Mid, int Count)
{
	static int TotalMove = 0;

	if (Count == 1)
	{
		cout << static_cast<int>(Start) << "�� �ִ� ���� " << static_cast<int>(Dest) << "�� �̵�." << endl;
		TotalMove++;
		return 0;
	}

	TowerOfHanoi(Start, Mid, Dest, Count - 1);

	cout << static_cast<int>(Start) << "�� �ִ� ���� " << static_cast<int>(Dest) << "�� �̵�." << endl;
	TotalMove++;

	TowerOfHanoi(Mid, Dest, Start, Count - 1);

	return TotalMove;
}