#pragma once

#include <map>

enum class Cell
{
	Empty,
	Grass,
	Water,
	Wall
};

class Board
{
public:

	Board();
	Board(int InRow, int InColume);
	Board(const Board& other) = delete;
	~Board();

	void DrawBoard();
	void UserInput();

private:

	void SetCell(int InRow, int InColume, Cell InData);
	Cell GetCell(int InRow, int InColume);
	void InitBoard();

private:

	std::map<Cell, char> CellMap;
	Cell** BoardMatrix;
	int BoardRow;
	int BoardColume;
	int CurRow;
	int CurColume;

};
