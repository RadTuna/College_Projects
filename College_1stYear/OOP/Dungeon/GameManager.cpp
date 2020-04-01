#include "GameManager.h"
#include "Timer.h"
#include "RenderUtil.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>

GameManager::GameManager(int BoardRow, int BoardColume, int InSeedInterval, int InMonsterInterval, int InitRow, int InitColume)
	: SeedTimer(nullptr),
	MonsterTimer(nullptr),
	GameBoard(nullptr),
	UserRow(InitRow),
	UserColume(InitColume),
	SeedInterval(InSeedInterval),
	MonsterInterval(InMonsterInterval),
	GameScore(0),
	IsGameEnd(false)
{
	GameBoard = new Board(BoardRow, BoardColume);
	SeedTimer = new Timer();
	MonsterTimer = new Timer();

	srand(static_cast<unsigned int>(time(nullptr)));

	TilePair[Tile::Empty] = ' ';
	TilePair[Tile::Seed] = '*';
	TilePair[Tile::Monster] = '!';
}

GameManager::~GameManager()
{
	if (MonsterTimer != nullptr)
	{
		delete MonsterTimer;
		MonsterTimer = nullptr;
	}

	if (SeedTimer != nullptr)
	{
		delete SeedTimer;
		SeedTimer = nullptr;
	}

	if (GameBoard != nullptr)
	{
		delete GameBoard;
		GameBoard = nullptr;
	}
}

void GameManager::Awake()
{
	SeedTimer->SetTimer(SeedInterval, this, GenerateSeed);
	MonsterTimer->SetTimer(MonsterInterval, this, GenerateMonster);

	ScreenInit();
}

void GameManager::Update()
{
	ScreenClear();

	UserInput();
	ProcessScore();
	DrawScreen();

	ScreenFlipping();
}

void GameManager::End()
{
	MonsterTimer->ClearTimer();
	SeedTimer->ClearTimer();

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
			UserRow--;
			break;
		case RIGHT:
			UserRow++;
			break;
		case UP:
			UserColume--;
			break;
		case DOWN:
			UserColume++;
			break;
		}
	}

	if (UserRow < 0)
	{
		UserRow = 0;
	}
	else if (UserRow >= GameBoard->GetMaxRow())
	{
		UserRow = GameBoard->GetMaxRow() - 1;
	}

	if (UserColume < 0)
	{
		UserColume = 0;
	}
	else if (UserColume >= GameBoard->GetMaxColume())
	{
		UserColume = GameBoard->GetMaxColume() - 1;
	}
}

void GameManager::DrawScreen()
{
	enum { BUF_SIZE = 16 };

	char StringBuf[BUF_SIZE];

	sprintf_s(StringBuf, "Score : %d", GameScore);
	ScreenPrint(0, 0, StringBuf);
	for (int Row = 0; Row < GameBoard->GetMaxRow(); ++Row)
	{
		for (int Colume = 0; Colume < GameBoard->GetMaxColume(); ++Colume)
		{
			if (Row == UserRow && Colume == UserColume)
			{
				sprintf_s(StringBuf, " @");
				ScreenPrint(Row * 2, Colume + 1, StringBuf);
				continue;
			}

			std::map<Tile, char>::iterator iter = TilePair.find(GameBoard->GetTile(Row, Colume));
			if (iter == TilePair.end())
			{
				continue;
			}
			sprintf_s(StringBuf, " %c", iter->second);
			ScreenPrint(Row * 2, Colume + 1, StringBuf);
		}
	}
}

void GameManager::ProcessScore()
{
	switch (GameBoard->GetTile(UserRow, UserColume))
	{
	case Tile::Seed:
		GameBoard->SetTile(UserRow, UserColume, Tile::Empty);
		GameScore++;
		break;
	case Tile::Monster:
		GameBoard->SetTile(UserRow, UserColume, Tile::Empty);
		IsGameEnd = true;
		break;
	}
}

void GameManager::GenerateSeed(void* InThis)
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
		if (Row == pThis->UserRow && Colume == pThis->UserColume)
		{
			continue;
		}

		pThis->GameBoard->SetTile(Row, Colume, Tile::Seed);
		break;
	}
}

void GameManager::GenerateMonster(void* InThis)
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
		if (Row == pThis->UserRow && Colume == pThis->UserColume)
		{
			continue;
		}

		pThis->GameBoard->SetTile(Row, Colume, Tile::Monster);
		break;
	}
}
