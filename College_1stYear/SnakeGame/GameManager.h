#pragma once

#include <map>
#include "Board.h"
#include "Snake.h"

class GameManager
{
public:

	GameManager(int BoardRow, int BoardColume, int InGameInterval, int InAppleInterval, int InitRow = 5, int InitColume = 5);
	~GameManager();

	void Awake();
	void Update();
	void End();

private:

	void UserInput();
	void DrawScreen();
	void DrawSnake();
	void ProcessCollision();
	static void ApplyInput(void* InThis);
	static void GenerateApple(void* InThis);

private:

	std::map<Tile, char> TilePair;
	class Timer* AppleTimer;
	class Timer* GameTimer;
	Board* GameBoard;
	Snake PlayerSnake;
	int AppleInterval;
	int GameInterval;
	bool IsGameEnd;

public:

	__forceinline bool GetIsGameEnd() const { return IsGameEnd; }

};
