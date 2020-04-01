#include "GameManager.h"
#include "Map.h"
#include "Cell.h"
#include <iostream>
#include <iomanip>

GameManager::GameManager()
{
	GameMap = nullptr;
	RemainCell = 0;
	IsGameEnd = false;
	IsDebugMode = false;
	IsGameWin = false;
}

GameManager::~GameManager()
{
}

void GameManager::InitGame()
{
	GameMap = new Map;
	if (GameMap == nullptr)
	{
		return;
	}

	int Row = 0;
	int Colume = 0;
	int MineNum = 0;

	std::cout << "���� ���� ��ü ũ�� �Է�" << std::endl;
	std::cin >> Row >> Colume;

	while (true)
	{
		std::cout << "���� ���� �Է�" << std::endl;
		std::cin >> MineNum;
		if (MineNum < Row * Colume)
		{
			break;
		}
	}

	RemainCell = Row * Colume - MineNum;

	GameMap->GenerateMap(Row, Colume);
	GameMap->GenerateMine(MineNum);
}

void GameManager::UpdateGame()
{
	// Update�� �Ǹ� �ܼ��� Ŭ�����մϴ�.
	system("cls");

	// ���� ���Ӻ��带 ����մϴ�.
	DrawConsole();

	// �������� �Է��� �޾� Map�� �����մϴ�.
	UserInput();

}

void GameManager::EndGame()
{
	system("cls");

	if (IsGameWin == true)
	{
		std::cout << "���ӿ��� �̰���ϴ�!!" << std::endl;
	}
	else
	{
		std::cout << "���ڸ� �����߽��ϴ�!!" << std::endl;
	}

	// ���� �� ���� ���� ���带 �����ִ� �뵵
	DrawConsole();

	GameMap->ReleaseMap();

	if (GameMap != nullptr)
	{
		delete GameMap;
		GameMap = nullptr;
	}
}

void GameManager::UserInput()
{
	int Row = 0;
	int Colume = 0;

	std::cout << "������ ��ǥ �Է�" << std::endl;

	std::cin >> Row >> Colume;

	if (GameMap->GetMaxRow() < Row || GameMap->GetMaxColume() < Colume)
	{
		std::cout << "��ǥ ������ ���� �ʽ��ϴ�." << std::endl;
		return;
	}

	Cell* CurCell = GameMap->GetCell(Row, Colume);

	if (CurCell->GetIsOpen() == true)
	{
		std::cout << "�̹� ���µ� ���Դϴ�." << std::endl;
		return;
	}

	RemainCell--;
	if (RemainCell <= 0)
	{
		IsGameEnd = true;
		IsGameWin = true;
	}
	if (CurCell->GetIsMine() == true)
	{
		IsGameEnd = true;
		IsGameWin = false;
	}

	GameMap->OpenCell(Row, Colume);
}

void GameManager::DrawConsole()
{
	if (IsDebugMode == true)
	{
		std::cout << "** ����� ���� ���� �� **" << std::endl;
	}

	// �� ��ǥ ���
	std::cout <<"��ǥ ";
	for (int i = 0; i < GameMap->GetMaxColume(); ++i)
	{
		std::cout << std::setw(2) << i;
	}
	std::cout << std::endl;

	// �� ��ǥ �� ���Ӻ��� ���;
	for (int Row = 0; Row < GameMap->GetMaxRow(); ++Row)
	{
		std::cout << std::setw(2) << Row << " : ";

		for (int Col = 0; Col < GameMap->GetMaxColume(); ++Col)
		{
			Cell* CurCell = GameMap->GetCell(Row, Col);

			if (CurCell->GetIsOpen() == false && IsDebugMode == false)
			{
				std::cout << std::setw(2) << "?";
				continue;
			}

			if (CurCell->GetIsMine() == true)
			{
				std::cout << std::setw(2) << "@";
			}
			else
			{
				std::cout << std::setw(2) << CurCell->GetNearMine();
			}
		}

		std::cout << std::endl;
	}
}
