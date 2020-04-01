#pragma once

#include <map>
#include "Queue.h"
#include "Point.h"

enum class Cell
{
	Empty,
	Wall,
	Path,
	Error
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
	void AddPath(int EndRow, int EndColume);
	void ClearPathMatrix();

private:

	std::map<Cell, char> CellMap;
	Cell** BoardMatrix;
	bool** FindMatrix;
	Point** PathMatrix;
	Queue<Point> PathQueue;

	Point BoardCoord;
	Point CursorCoord;
	Point StartPoint;
	Point EndPoint;

};
