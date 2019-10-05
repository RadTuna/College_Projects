#include <iostream>
#include "GameManager.h"


// ���� ���� �뷮 10MB
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