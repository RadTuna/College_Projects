#pragma once

class GameManager
{
public:

	GameManager();
	~GameManager();

	void InitGame();
	void UpdateGame();
	void EndGame();

	void UserInput();
	void DrawConsole();

	void FindEmptyCell(int Row, int Colume);

private:

	class Map* GameMap;
	int RemainCell;
	bool IsGameEnd;
	bool IsGameWin;
	bool IsDebugMode;
	int RecursiveCallCount;

public:

	inline bool GetIsGameEnd() const { return IsGameEnd; }

};
