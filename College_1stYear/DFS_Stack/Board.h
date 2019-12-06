#pragma once

#include <map>
#include "Stack.h"
#include "Point.h"

enum class Cell
{
	Empty,
	Wall,
	Path
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
	void ClearBoard();
	bool PathFind(int StartRow, int StartColume, int EndRow, int EndColume);
	void AddPath();

private:

	std::map<Cell, char> CellMap;
	Cell** BoardMatrix;
	bool** FindMatrix;
	Stack<Point> PathStack;

	Point BoardCoord;
	Point CursorCoord;
	Point StartPoint;
	Point EndPoint;

};
