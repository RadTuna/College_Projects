#include <iostream>
#include <ctime>
#include "Revolver.h"
#include "RouletteManager.h"

bool RouletteManager::IsRandPlayer = true;

bool RouletteManager::IsRandShuffle = true;

bool RouletteManager::IsLastOneMode = false;

void RouletteManager::SetMode(bool RandPlayer, bool RandShuffle, bool LastOne)
{
	IsRandPlayer = RandPlayer;
	IsRandShuffle = RandShuffle;
	IsLastOneMode = LastOne;
}

void RouletteManager::PlayRoulette(Revolver& Pistol, int MaxPlayer)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	int NowPlayer = 1;

	if (IsRandPlayer == true)
	{
		NowPlayer = rand() % MaxPlayer + 1;
	}

	while (true)
	{
		std::cout << NowPlayer << "번 플레이어 차례입니다." << std::endl;
		std::cin.get();

		if (Pistol.Bang() == true)
		{
			std::cout << "Bang!" << std::endl;
			std::cout << NowPlayer << "번 플레이어가 사망했습니다." << std::endl;
			MaxPlayer--;
			if (IsLastOneMode == false)
			{
				std::cout << "게임이 종료되었습니다." << std::endl;
				return;
			}
			else if (Pistol.IsEmpty() == true)
			{
				std::cout << "게임이 종료되었습니다." << std::endl;
				return;
			}
		}
		else
		{
			std::cout << "철컥!" << std::endl;
		}
		NowPlayer++;
		if (NowPlayer > MaxPlayer)
		{
			NowPlayer = 1;
		}

		if (IsRandShuffle == true)
		{
			Pistol.CylinderRotation();
		}
	}
}