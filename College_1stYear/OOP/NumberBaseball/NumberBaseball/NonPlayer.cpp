#include "NonPlayer.h"
#include <ctime>
#include <cstdlib>
#include <iostream>


NonPlayer::NonPlayer()
{
	srand(static_cast<unsigned int>(std::time(0)));
}

void NonPlayer::InputNumber()
{
	int GenNumber = rand() % 990 + 10;

	if (CheckNumber(GenNumber) == false)
	{
		InputNumber();
		return;
	}

	// std::cout << GenNumber << " - AINum:DebugLog" << std::endl;

	SetCurrentNumber(GenNumber);
}
