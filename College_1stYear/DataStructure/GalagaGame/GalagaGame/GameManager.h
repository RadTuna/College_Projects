#pragma once

#include <map>
#include "Board.h"
#include "Timer.h"
#include "Player.h"
#include "SharedList.h"

class GameManager
{
public:

	GameManager(int BoardRow, int BoardColume, int InEnemyInterval, int InBulletInterval, int InitRow = 5, int InitColume = 5);
	~GameManager() = default;

	void Awake();
	void Update();
	void End();

private:

	void UserInput();
	void DrawScreen();
	void ProcessCollision();
	static void UpdateBullet(void* InThis);
	static void UpdateEnemy(void* InThis);

private:

	std::map<std::string, char> TilePair;
	std::unique_ptr<Timer> BulletTimer;
	std::unique_ptr<Timer> EnemyTimer;
	std::unique_ptr<Board> GameBoard;
	std::unique_ptr<Player> GamePlayer;
	std::unique_ptr<SharedList> BulletList;
	std::unique_ptr<SharedList> EnemyList;
	int EnemyInterval;
	int BulletIntetval;
	bool IsGameEnd;

public:

	__forceinline bool GetIsGameEnd() const { return IsGameEnd; }

};
