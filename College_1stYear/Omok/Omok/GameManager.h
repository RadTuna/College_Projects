#pragma once


class GameManger
{
public:

	GameManger(int InBoardSize);
	~GameManger();

	void Awake();
	void Update();
	void Destroy();

	void DrawGameBoard();
	void InputCoordnate();
	bool IsOmok();

private:

	int FindOmok(int Voffset, int Hoffset, int Count = 0);
	int Sign(int Value);

private:

	class Board* GameLevel;
	bool IsBlackUser;
	int SelectRow;
	int SelectColume;

};
