#include <iostream>
#include "GameManager.h"


int main()
{
	GameManager* Game = new GameManager;
	if (Game == nullptr)
	{
		return 0;
	}

	Game->GameStart();

	while (Game->GetIsGameEnd() == false)
	{
		Game->GameUpdate();
	}

	delete Game;
	Game = nullptr;

	return 0;
}