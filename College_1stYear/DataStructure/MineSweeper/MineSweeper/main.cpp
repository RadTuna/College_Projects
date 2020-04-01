#include <iostream>
#include "GameManager.h"


// 스택 예약 용량 10MB
int main()
{
	GameManager MineSweeper;

	MineSweeper.InitGame();

	while (MineSweeper.GetIsGameEnd() == false)
	{
		MineSweeper.UpdateGame();
	}

	MineSweeper.EndGame();

	return 0;
}