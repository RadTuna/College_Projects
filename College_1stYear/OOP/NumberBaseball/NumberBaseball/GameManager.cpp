#include "GameManager.h"
#include "Player.h"
#include "NonPlayer.h"
#include <iostream>
#include <ctime>

GameManager::GameManager()
{
	Pitcher = new Player;
	if (Pitcher == nullptr)
	{
		return;
	}

	Hitter = new NonPlayer;
	if (Hitter == nullptr)
	{
		return;
	}

	IsGameEnd = false;
}

GameManager::~GameManager()
{
	if (Pitcher != nullptr)
	{
		delete Pitcher;
		Pitcher = nullptr;
	}

	if (Hitter != nullptr)
	{
		delete Hitter;
		Hitter = nullptr;
	}
}

bool GameManager::CompareNumber(int& OutStrike, int& OutBall) const
{
	int PitcherNumbers[3];
	PitcherNumbers[0] = Pitcher->GetCurrentNumber() / 100;
	PitcherNumbers[1] = (Pitcher->GetCurrentNumber() / 10) % 10;
	PitcherNumbers[2] = Pitcher->GetCurrentNumber() % 10;

	int HitterNumbers[3];
	HitterNumbers[0] = Hitter->GetCurrentNumber() / 100;
	HitterNumbers[1] = (Hitter->GetCurrentNumber() / 10) % 10;
	HitterNumbers[2] = Hitter->GetCurrentNumber() % 10;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (PitcherNumbers[i] == HitterNumbers[j])
			{
				OutBall++;
			}
		}

		if (PitcherNumbers[i] == HitterNumbers[i])
		{
			OutStrike++;
		}
	}

	OutBall -= OutStrike;

	if (OutBall == 0 && OutStrike == 0)
	{
		return false;
	}

	return true;
}

void GameManager::GameStart()
{
	std::cout << "���ھ߱� ������ �����մϴ�." << std::endl;
	std::cout << "���ڴ� 3�ڸ��� �ߺ����� �ʴ� ���ڷ� ���ѵ˴ϴ�." << std::endl;

	// AI�� ������ �����.
	Hitter->InputNumber();
}

void GameManager::GameUpdate()
{
	// ������ �Է��� ����.
	Pitcher->InputNumber();

	PrintResult();
}

void GameManager::PrintResult()
{
	int Strike = 0;
	int Ball = 0;

	if (CompareNumber(Strike, Ball) == false)
	{
		std::cout << "�ƿ�!" << std::endl;
		return;
	}

	std::cout << Strike << "��Ʈ����ũ! / " << Ball << "��!" << std::endl;

	if (Strike >= 3)
	{
		IsGameEnd = true;
	}
}
