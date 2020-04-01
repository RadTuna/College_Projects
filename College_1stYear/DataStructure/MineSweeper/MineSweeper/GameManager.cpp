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

	RecursiveCallCount = 0;
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

	std::cout << "게임 맵의 전체 크기 입력" << std::endl;
	std::cin >> Row >> Colume;

	while (true)
	{
		std::cout << "지뢰 개수 입력" << std::endl;
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
	// Update가 되면 콘솔을 클리어합니다.
	system("cls");

	// 현재 게임보드를 출력합니다.
	DrawConsole();
	// std::cout << RecursiveCallCount << std::endl;

	// 유저에게 입력을 받아 Map을 갱신합니다.
	UserInput();

}

void GameManager::EndGame()
{
	system("cls");

	if (IsGameWin == true)
	{
		std::cout << "게임에서 이겼습니다!!" << std::endl;
	}
	else
	{
		std::cout << "지뢰를 선택했습니다!!" << std::endl;
	}

	// 종료 시 최종 게임 보드를 보여주는 용도
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

	std::cout << "선택한 좌표 입력" << std::endl;

	std::cin >> Row >> Colume;

	if (Row == -22 && Colume == -55)
	{
		IsDebugMode = !IsDebugMode;
		return;
	}

	if (GameMap->GetMaxRow() <= Row || GameMap->GetMaxColume() <= Colume)
	{
		std::cout << "좌표 범위가 맞지 않습니다." << std::endl;
		return;
	}

	Cell* CurCell = GameMap->GetCell(Row, Colume);

	if (CurCell->GetIsOpen() == true)
	{
		std::cout << "이미 오픈된 셀입니다." << std::endl;
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

	if (GameMap->GetCell(Row, Colume)->GetNearMine() == 0)
	{
		FindEmptyCell(Row, Colume);
	}
	else
	{
		GameMap->OpenCell(Row, Colume);
	}
}

void GameManager::DrawConsole()
{
	if (IsDebugMode == true)
	{
		std::cout << "** 디버그 모드로 동작 중 **" << std::endl;
	}

	// 열 좌표 출력
	std::cout <<"좌표 ";
	for (int i = 0; i < GameMap->GetMaxColume(); ++i)
	{
		std::cout << std::setw(2) << i;
	}
	std::cout << std::endl;

	// 행 좌표 및 게임보드 출력;
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

void GameManager::FindEmptyCell(int Row, int Colume)
{
	// 접근한 셀이 유효하지 않은 셀이면 재귀탈출 (배열 범위 이탈 시)
	if (GameMap->GetCell(Row, Colume) == nullptr)
	{
		return;
	}

	// 접근한 셀이 이미 오픈된 셀이면 재귀탈출
	if (GameMap->GetCell(Row, Colume)->GetIsOpen() == true)
	{
		return;
	}

	// 접근한 셀의 값이 0이 아니면 재귀탈출
	if (GameMap->GetCell(Row, Colume)->GetNearMine() != 0)
	{
		return;
	}

	GameMap->OpenCell(Row, Colume);
	RecursiveCallCount++;

	// 주변의 4개의 셀에 재귀호출 실시.
	FindEmptyCell(Row - 1, Colume);
	FindEmptyCell(Row, Colume - 1);
	FindEmptyCell(Row, Colume + 1);
	FindEmptyCell(Row + 1, Colume);
}
