#include "GameManager.h"
#include "RenderUtil.h"
#include "Bullet.h"
#include "Enemy.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>

GameManager::GameManager(int BoardRow, int BoardColume, int InEnemyInterval, int InBulletInterval, int InitRow, int InitColume)
	: BulletTimer(nullptr)
	, EnemyTimer(nullptr)
	, GameBoard(nullptr)
	, IsGameEnd(false)
	, EnemyInterval(InEnemyInterval)
	, BulletIntetval(InBulletInterval)
{
	GameBoard = std::make_unique<Board>(BoardRow, BoardColume);
	BulletTimer = std::make_unique<Timer>();
	EnemyTimer = std::make_unique<Timer>();
	GamePlayer = std::make_unique<Player>(InitRow, InitColume);
	BulletList = std::make_unique<SharedList>();
	EnemyList = std::make_unique<SharedList>();

	srand(static_cast<unsigned int>(time(nullptr)));

	TilePair["Entity"] = ' ';
	TilePair["Player"] = '&';
	TilePair["Bullet"] = '*';
	TilePair["Enemy"] = '@';
}

void GameManager::Awake()
{
	BulletTimer->SetTimer(BulletIntetval, this, &GameManager::UpdateBullet);
	EnemyTimer->SetTimer(EnemyInterval, this, &GameManager::UpdateEnemy);

	ScreenInit();
}

void GameManager::Update()
{
	ScreenClear();

	UserInput();
	ProcessCollision();
	DrawScreen();

	ScreenFlipping();
}

void GameManager::End()
{
	BulletTimer->ClearTimer();
	EnemyTimer->ClearTimer();

	ScreenRelease();
}

void GameManager::UserInput()
{
	enum { LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACE = 32 };

	if (_kbhit())
	{
		char InChar = _getch();

		if (InChar == 224)
		{
			InChar = _getch();
		}

		switch (InChar)
		{
		case LEFT:
			GameBoard->SetTile(GamePlayer->GetPosX(), GamePlayer->GetPosY(), Entity());
			GamePlayer->MoveLeft(1, 0);
			break;
		case RIGHT:
			GameBoard->SetTile(GamePlayer->GetPosX(), GamePlayer->GetPosY(), Entity());
			GamePlayer->MoveRight(1, GameBoard->GetMaxColume() - 1);
			break;
		case SPACE:
			if (GameBoard->GetTile(GamePlayer->GetPosX(), GamePlayer->GetPosY() - 1).GetTag() != "Bullet")
			{
				BulletList->Insert(0, Bullet(GamePlayer->GetPosX(), GamePlayer->GetPosY() - 1));
				GameBoard->SetTile(GamePlayer->GetPosX(), GamePlayer->GetPosY() - 1, Bullet());
			}
			break;
		}
	}

	GameBoard->SetTile(GamePlayer->GetPosX(), GamePlayer->GetPosY(), *GamePlayer.get());
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

			std::map<std::string, char>::iterator iter = TilePair.find(GameBoard->GetTile(Row - 1, Colume - 1).GetTag());
			if (iter == TilePair.end())
			{
				continue;
			}
			sprintf_s(StringBuf, " %c", iter->second);
			ScreenPrint(Row * 2, Colume, StringBuf);
		}
	}
}

void GameManager::ProcessCollision()
{
	for (int i = 0; i < BulletList->GetLength(); ++i)
	{
		Entity CurBullet;
		if (BulletList->Retrieve(i, CurBullet) == false)
		{
			continue;
		}

		for (int j = 0; j < EnemyList->GetLength(); ++j)
		{
			Entity CurEnemy;
			if (EnemyList->Retrieve(j, CurEnemy) == false)
			{
				continue;
			}

			if (CurBullet.GetPosX() == CurEnemy.GetPosX() && CurBullet.GetPosY() == CurEnemy.GetPosY())
			{
				GameBoard->SetTile(CurBullet.GetPosX(), CurBullet.GetPosY(), Entity());
				GameBoard->SetTile(CurEnemy.GetPosX(), CurEnemy.GetPosY(), Entity());
				BulletList->Remove(i);
				EnemyList->Remove(j);
			}
		}
	}
}

void GameManager::UpdateBullet(void* InThis)
{
	GameManager* pThis = reinterpret_cast<GameManager*>(InThis);

	for (int i = 0; i < pThis->BulletList->GetLength(); ++i)
	{
		Entity* TempEntity;
		TempEntity = pThis->BulletList->RetrievePtr(i);
		if (TempEntity == nullptr)
		{
			continue;
		}
		if (TempEntity->GetPosY() < 0)
		{
			pThis->BulletList->Remove(i);
			--i;
			continue;
		}

		pThis->GameBoard->SetTile(TempEntity->GetPosX(), TempEntity->GetPosY(), Entity());
		pThis->GameBoard->SetTile(TempEntity->GetPosX(), TempEntity->GetPosY() - 1, Bullet());
		TempEntity->SetPosition(TempEntity->GetPosX(), TempEntity->GetPosY() - 1);
	}
}

void GameManager::UpdateEnemy(void* InThis)
{
	GameManager* pThis = reinterpret_cast<GameManager*>(InThis);

	static int MoveCount = 2;
	static bool IsMoveLeft = true;

	if (pThis->EnemyList->GetLength() <= 0)
	{
		pThis->EnemyList->Insert(0, Enemy(3, 0));
		pThis->GameBoard->SetTile(3, 0, Enemy());
		pThis->EnemyList->Insert(0, Enemy(4, 0));
		pThis->GameBoard->SetTile(4, 0, Enemy());
		pThis->EnemyList->Insert(0, Enemy(5, 0));
		pThis->GameBoard->SetTile(5, 0, Enemy());
		pThis->EnemyList->Insert(0, Enemy(6, 0));
		pThis->GameBoard->SetTile(6, 0, Enemy());
		return;
	}

	int MoveOffset = 0;
	if (MoveCount <= 0)
	{
		IsMoveLeft = !IsMoveLeft;
		MoveCount = 4;
	}
	if (IsMoveLeft == true)
	{
		MoveOffset = -1;

		for (int i = pThis->EnemyList->GetLength(); i >= 0 ; --i)
		{
			Entity* TempEntity;
			TempEntity = pThis->EnemyList->RetrievePtr(i);

			if (TempEntity == nullptr)
			{
				continue;
			}

			pThis->GameBoard->SetTile(TempEntity->GetPosX(), TempEntity->GetPosY(), Entity());
			pThis->GameBoard->SetTile(TempEntity->GetPosX() + MoveOffset, TempEntity->GetPosY(), Enemy());
			TempEntity->SetPosition(TempEntity->GetPosX() + MoveOffset, TempEntity->GetPosY());
		}
	}
	else
	{
		MoveOffset = 1;

		for (int i = 0; i < pThis->EnemyList->GetLength(); ++i)
		{
			Entity* TempEntity;
			TempEntity = pThis->EnemyList->RetrievePtr(i);

			if (TempEntity == nullptr)
			{
				continue;
			}

			pThis->GameBoard->SetTile(TempEntity->GetPosX(), TempEntity->GetPosY(), Entity());
			pThis->GameBoard->SetTile(TempEntity->GetPosX() + MoveOffset, TempEntity->GetPosY(), Enemy());
			TempEntity->SetPosition(TempEntity->GetPosX() + MoveOffset, TempEntity->GetPosY());
		}
	}
	MoveCount--;
}

