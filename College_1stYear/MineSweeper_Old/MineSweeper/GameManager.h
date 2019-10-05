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

private:

	class Map* GameMap;
	int RemainCell;
	bool IsGameEnd;
	bool IsGameWin;
	bool IsDebugMode;

public:

	inline bool GetIsGameEnd() const { return IsGameEnd; }

};
