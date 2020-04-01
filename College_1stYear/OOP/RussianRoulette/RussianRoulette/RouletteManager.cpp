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
		std::cout << NowPlayer << "�� �÷��̾� �����Դϴ�." << std::endl;
		std::cin.get();

		if (Pistol.Bang() == true)
		{
			std::cout << "Bang!" << std::endl;
			std::cout << NowPlayer << "�� �÷��̾ ����߽��ϴ�." << std::endl;
			MaxPlayer--;
			if (IsLastOneMode == false)
			{
				std::cout << "������ ����Ǿ����ϴ�." << std::endl;
				return;
			}
			else if (Pistol.IsEmpty() == true)
			{
				std::cout << "������ ����Ǿ����ϴ�." << std::endl;
				return;
			}
		}
		else
		{
			std::cout << "ö��!" << std::endl;
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