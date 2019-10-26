#include "GameManager.h"
#include "Timer.h"
#include "RenderUtil.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>

GameManager::GameManager(int BoardRow, int BoardColume, int InGameInterval, int InAppleInterval, int InitRow, int InitColume)
	: AppleTimer(nullptr),
	GameTimer(nullptr),
	GameBoard(nullptr),
	PlayerSnake(InitRow, InitColume),
	AppleInterval(InAppleInterval),
	GameInterval(InGameInterval),
	IsGameEnd(false)
{
	GameBoard = new Board(BoardRow, BoardColume);
	AppleTimer = new Timer;
	GameTimer = new Timer;

	srand(static_cast<unsigned int>(time(nullptr)));

	TilePair[Tile::Empty] = ' ';
	TilePair[Tile::Apple] = '*';
}

GameManager::~GameManager()
{
	if (AppleTimer != nullptr)
	{
		delete AppleTimer;
		AppleTimer = nullptr;
	}

	if (GameBoard != nullptr)
	{
		delete GameBoard;
		GameBoard = nullptr;
	}
}

void GameManager::Awake()
{
	AppleTimer->SetTimer(AppleInterval, this, GenerateApple);
	GameTimer->SetTimer(GameInterval, this, ApplyInput);

	ScreenInit();
}

void GameManager::Update()
{
	ScreenClear();

	UserInput();
	ProcessCollision();
	DrawScreen();
	DrawSnake();

	ScreenFlipping();
}

void GameManager::End()
{
	AppleTimer->ClearTimer();
	GameTimer->ClearTimer();

	ScreenRelease();
}

void GameManager::UserInput()
{
	enum { LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

	if (_kbhit())
	{
		char InChar = _getch();
		switch (InChar)
		{
		case LEFT:
			if (PlayerSnake.GetSnakeHeading() == Heading::RIGHT)
			{
				break;
			}
			PlayerSnake.SetSnakeHeading(Heading::LEFT);
			break;
		case RIGHT:
			if (PlayerSnake.GetSnakeHeading() == Heading::LEFT)
			{
				break;
			}
			PlayerSnake.SetSnakeHeading(Heading::RIGHT);
			break;
		case UP:
			if (PlayerSnake.GetSnakeHeading() == Heading::DOWN)
			{
				break;
			}
			PlayerSnake.SetSnakeHeading(Heading::UP);
			break;
		case DOWN:
			if (PlayerSnake.GetSnakeHeading() == Heading::UP)
			{
				break;
			}
			PlayerSnake.SetSnakeHeading(Heading::DOWN);
			break;
		}
	}
}

void GameManager::DrawScreen()
{
	enum { BUF_SIZE = 16 };

	char StringBuf[BUF_SIZE];
	for (int Row = 0; Row < GameBoard->GetMaxRow() + 2; ++Row)
	{
		for (int Colume = 0; Colume < GameBoard->GetMaxColume() + 2; ++Colume)
		{
			if (Row == 0 || Colume == 0)
			{
				ScreenPrint(Row * 2, Colume, const_cast<char*>(" #"));
				continue;
			}
			if (Row >= GameBoard->GetMaxRow() + 1 || Colume >= GameBoard->GetMaxColume() + 1)
			{
				ScreenPrint(Row * 2, Colume, const_cast<char*>(" #"));
				continue;
			}

			std::map<Tile, char>::iterator iter = TilePair.find(GameBoard->GetTile(Row - 1, Colume - 1));
			if (iter == TilePair.end())
			{
				continue;
			}
			sprintf_s(StringBuf, " %c", iter->second);
			ScreenPrint(Row * 2, Colume, StringBuf);
		}
	}
}

void GameManager::DrawSnake()
{
	for (int i = 0; i < PlayerSnake.GetSnakeList().GetLenght(); ++i)
	{
		const Point* CurPoint = PlayerSnake.GetSnakeList().Retrieve(i);

		if (CurPoint->X < 0 || CurPoint->Y < 0)
		{
			return;
		}

		ScreenPrint(CurPoint->X * 2 + 2, CurPoint->Y + 1, const_cast<char*>(" @"));
	}
}

void GameManager::ProcessCollision()
{
	const Point* HeadPoint = PlayerSnake.GetSnakeList().Retrieve(0);

	// 보드 외곽에 충돌하면 게임종료.
	if (HeadPoint->X < 0 || HeadPoint->X >= GameBoard->GetMaxColume())
	{
		IsGameEnd = true;
		return;
	}
	if (HeadPoint->Y < 0 || HeadPoint->Y >= GameBoard->GetMaxColume())
	{
		IsGameEnd = true;
		return;
	}

	// 자신의 Tail과 충돌하면 게임종료.
	for (int i = 1; i < PlayerSnake.GetSnakeList().GetLenght(); ++i)
	{
		const Point* TailPoint = PlayerSnake.GetSnakeList().Retrieve(i);
		if (HeadPoint->X == TailPoint->X && HeadPoint->Y == TailPoint->Y)
		{
			IsGameEnd = true;
			return;
		}
	}

	// 사과와 충돌하면 진행방향 1칸 앞에 Tail추가.
	if (GameBoard->GetTile(HeadPoint->X, HeadPoint->Y) == Tile::Apple)
	{
		GameBoard->SetTile(HeadPoint->X, HeadPoint->Y, Tile::Empty);
		switch (PlayerSnake.GetSnakeHeading())
		{
		case Heading::UP:
			PlayerSnake.AddTail(HeadPoint->X, HeadPoint->Y - 1);
			break;
		case Heading::DOWN:
			PlayerSnake.AddTail(HeadPoint->X, HeadPoint->Y + 1);
			break;
		case Heading::LEFT:
			PlayerSnake.AddTail(HeadPoint->X - 1, HeadPoint->Y);
			break;
		case Heading::RIGHT:
			PlayerSnake.AddTail(HeadPoint->X + 1, HeadPoint->Y);
			break;
		}
	}
}

void GameManager::ApplyInput(void* InThis)
{
	GameManager* pThis = reinterpret_cast<GameManager*>(InThis);

	const Point* HeadPoint = pThis->PlayerSnake.GetSnakeList().Retrieve(0);

	switch (pThis->PlayerSnake.GetSnakeHeading())
	{
	case Heading::UP:
		pThis->PlayerSnake.UpdateTail(HeadPoint->X, HeadPoint->Y - 1);
		break;
	case Heading::DOWN:
		pThis->PlayerSnake.UpdateTail(HeadPoint->X, HeadPoint->Y + 1);
		break;
	case Heading::LEFT:
		pThis->PlayerSnake.UpdateTail(HeadPoint->X - 1, HeadPoint->Y);
		break;
	case Heading::RIGHT:
		pThis->PlayerSnake.UpdateTail(HeadPoint->X + 1, HeadPoint->Y);
		break;
	}
}

void GameManager::GenerateApple(void* InThis)
{
	GameManager* pThis = reinterpret_cast<GameManager*>(InThis);

	for (;;)
	{
		int Row = rand() % pThis->GameBoard->GetMaxRow();
		int Colume = rand() % pThis->GameBoard->GetMaxColume();

		if (pThis->GameBoard->GetTile(Row, Colume) != Tile::Empty)
		{
			continue;
		}

		// Tail과 겹치는 위치에는 사과를 스폰시키지 않음.
		for (int i = 0; i < pThis->PlayerSnake.GetSnakeList().GetLenght(); ++i)
		{
			const Point* CurPoint = pThis->PlayerSnake.GetSnakeList().Retrieve(i);
			if (CurPoint->X == Row && CurPoint->Y == Colume)
			{
				Row = rand() % pThis->GameBoard->GetMaxRow();
				Colume = rand() % pThis->GameBoard->GetMaxColume();
				i = 0;
				continue;
			}
		}

		pThis->GameBoard->SetTile(Row, Colume, Tile::Apple);
		break;
	}
}

