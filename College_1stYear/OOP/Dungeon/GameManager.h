#pragma once

#include <map>
#include "Board.h"

class GameManager
{
public:

	GameManager(int BoardRow, int BoardColume, int InSeedInterval, int InMonsterInterval, int InitRow = 0, int InitColume = 0);
	~GameManager();

	void Awake();
	void Update();
	void End();

private:

	void UserInput();
	void DrawScreen();
	void ProcessScore();
	static void GenerateSeed(void* InThis);
	static void GenerateMonster(void* InThis);

private:

	std::map<Tile, char> TilePair;
	class Timer* SeedTimer;
	class Timer* MonsterTimer;
	class Board* GameBoard;
	int UserRow;
	int UserColume;
	int GameScore;
	int SeedInterval;
	int MonsterInterval;
	bool IsGameEnd;

public:

	__forceinline bool GetIsGameEnd() const { return IsGameEnd; }

};
