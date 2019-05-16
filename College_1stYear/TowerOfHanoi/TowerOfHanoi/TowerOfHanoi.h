#pragma once

enum class Hanoi
{
	StartPoint = 1,
	MidPoint = 2,
	EndPoint = 3
};

int TowerOfHanoi(Hanoi Start, Hanoi Destiantion, Hanoi Mid, int Count);
