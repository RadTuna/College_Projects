#include "GameManager.h"
#include "Board.h"
#include <iostream>

using namespace std;

GameManger::GameManger(int InBoardSize)
	: IsBlackUser(true),
	SelectRow(0),
	SelectColume(0)
{
	GameLevel = new Board(InBoardSize);
}

GameManger::~GameManger()
{
	delete GameLevel;
	GameLevel = nullptr;
}

void GameManger::Awake()
{
	cout << "오목 게임을 시작합니다." << endl;
}

void GameManger::Update()
{
	DrawGameBoard();
	InputCoordnate();

	IsBlackUser = !IsBlackUser;
	system("cls");
}

void GameManger::Destroy()
{
	DrawGameBoard();

	IsBlackUser = !IsBlackUser;

	if (IsBlackUser == true)
	{
		cout << "흑이 승리하였습니다." << endl;
	}
	else
	{
		cout << "백이 승리하였습니다." << endl;
	}
}

void GameManger::DrawGameBoard()
{
	cout << " ";
	for (int j = 0; j < GameLevel->GetBoardSize() * 2; ++j)
	{
		cout << "━━";
	}
	cout << endl;

	for (int i = 0; i < GameLevel->GetBoardSize(); ++i)
	{
		cout << "┃";
		for (int j = 0; j < GameLevel->GetBoardSize(); ++j)
		{
			switch(GameLevel->GetBoardElement(i, j))
			{
			case EBoard::Black:
				cout << " ○";
				break;
			case EBoard::White:
				cout << " ●";
				break;
			case EBoard::Empty:
				cout << " 　";
				break;
			}
			cout << "┃";
		}
		cout << endl;

		cout << " ";
		for (int j = 0; j < GameLevel->GetBoardSize() * 2; ++j)
		{
			cout << "━━";
		}
		cout << endl;
	}
}

void GameManger::InputCoordnate()
{
	if (IsBlackUser == true)
	{
		cout << "흑의 차례입니다. ";
	}
	else
	{
		cout << "백의 차례입니다. ";
	}
	cout << "좌표를 입력하세요." << endl;
	cin >> SelectRow >> SelectColume;
	SelectRow--;
	SelectColume--;

	if (GameLevel->GetBoardElement(SelectRow, SelectColume) == EBoard::Empty)
	{
		GameLevel->SetBoardElemenet(SelectRow, SelectColume, IsBlackUser ? EBoard::Black : EBoard::White);
	}
	else
	{
		IsBlackUser = !IsBlackUser;
	}
}

bool GameManger::IsOmok()
{
	int HorizontalTotal = FindOmok(0, -1);
	int VerticalTotal = FindOmok(-1, 0);
	int LTCrossTotal = FindOmok(-1, -1);
	int RTCrossTotal = FindOmok(-1, 1);

	HorizontalTotal += FindOmok(0, 1);
	VerticalTotal += FindOmok(1, 0);
	LTCrossTotal += FindOmok(1, 1);
	RTCrossTotal += FindOmok(1, -1);

	// 현재 놓은 돌 제외 이어진 돌의 합이 4면 승리 // 따라서, 6목, 7목으로는 승리할 수 없음.
	if (HorizontalTotal == 4 || VerticalTotal == 4 || LTCrossTotal == 4 || RTCrossTotal == 4)
	{
		return true;
	}

	return false;
}

int GameManger::FindOmok(int Voffset, int Hoffset, int Count)
{
	if (Count >= 4)
	{
		return 4;
	}

	if (GameLevel->GetBoardElement(SelectRow + Voffset, SelectColume + Hoffset) != (!IsBlackUser ? EBoard::Black : EBoard::White))
	{
		return Count;
	}

	return FindOmok(Voffset + Sign(Voffset), Hoffset + Sign(Hoffset), ++Count);
}

int GameManger::Sign(int Value)
{
	if (Value > 0)
	{
		return 1;
	}
	else if (Value < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
