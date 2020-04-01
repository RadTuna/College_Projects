#include "Board.h"
#include "util.h"
#include <conio.h>
#include <iostream>

Board::Board()
	: BoardCoord(10, 10)
	, CursorCoord(0, 0)
	, BoardMatrix(nullptr)
	, FindMatrix(nullptr)

{
	InitBoard();
}

Board::Board(int InRow, int InColume)
	: BoardCoord(InColume, InRow)
	, CursorCoord(0, 0)
	, BoardMatrix(nullptr)
	, FindMatrix(nullptr)
{
	InitBoard();
}

Board::~Board()
{
	if (BoardMatrix != nullptr)
	{
		for (int i = 0; i < BoardCoord.Y; ++i)
		{
			if (BoardMatrix[i] != nullptr)
			{
				delete[] BoardMatrix[i];
				BoardMatrix[i] = nullptr;
			}
		}

		delete[] BoardMatrix;
		BoardMatrix = nullptr;
	}

	if (FindMatrix != nullptr)
	{
		for (int i = 0; i < BoardCoord.Y; ++i)
		{
			if (FindMatrix[i] != nullptr)
			{
				delete[] FindMatrix[i];
				FindMatrix[i] = nullptr;
			}
		}

		delete[] FindMatrix;
		FindMatrix = nullptr;
	}

	if (PathMatrix != nullptr)
	{
		for (int i = 0; i < BoardCoord.Y; ++i)
		{
			if (PathMatrix[i] != nullptr)
			{
				delete[] PathMatrix[i];
				PathMatrix[i] = nullptr;
			}

			delete[] PathMatrix;
			PathMatrix = nullptr;
		}
	}
}

void Board::DrawBoard()
{
	char StringBuf[32];
	sprintf_s(StringBuf, "StrX : %d", StartPoint.X);
	ScreenPrint(0, 0, StringBuf);
	sprintf_s(StringBuf, "StrY : %d", StartPoint.Y);
	ScreenPrint(0, 1, StringBuf);
	sprintf_s(StringBuf, "EndX : %d", EndPoint.X);
	ScreenPrint(0, 2, StringBuf);
	sprintf_s(StringBuf, "EndY : %d", EndPoint.Y);
	ScreenPrint(0, 3, StringBuf);

	for (int i = 0; i < BoardCoord.Y; ++i)
	{
		for (int j = 0; j < BoardCoord.X; ++j)
		{
			std::map<Cell, char>::iterator iter = CellMap.find(GetCell(j, i));
			if (iter == CellMap.end())
			{
				continue;
			}

			char CharBuf[2];
			CharBuf[0] = iter->second;
			CharBuf[1] = '\0';
			ScreenPrint(i * 2, j + 5, CharBuf);
		}
	}

	ScreenPrint(CursorCoord.X * 2, CursorCoord.Y + 5, const_cast<char*>("^"));
	ScreenPrint(StartPoint.X * 2, StartPoint.Y + 5, const_cast<char*>("*"));
	ScreenPrint(EndPoint.X * 2, EndPoint.Y + 5, const_cast<char*>("*"));
}

void Board::UserInput()
{
	if (_kbhit())
	{
		int CurInput = _getch();
		bool FindResult = false;
		switch (CurInput)
		{
		case LEFT:
			CursorCoord.X--;
			break;
		case RIGHT:
			CursorCoord.X++;
			break;
		case UP:
			CursorCoord.Y--;
			break;
		case DOWN:
			CursorCoord.Y++;
			break;
		case ONE:
			StartPoint = CursorCoord;
			break;
		case TWO:
			EndPoint = CursorCoord;
			break;
		case THREE:
			SetCell(CursorCoord.Y, CursorCoord.X, Cell::Wall);
			break;
		case ENTER:
			FindResult = PathFind(StartPoint.Y, StartPoint.X, EndPoint.Y, EndPoint.X);
			if (FindResult == true)
			{
				AddPath(EndPoint.Y, EndPoint.X);
			}
			break;
		case SPACE:
			ClearBoard();
			break;
		}

		if (CursorCoord.Y < 0)
		{
			CursorCoord.Y = 0;
		}
		else if (CursorCoord.Y > BoardCoord.Y - 1)
		{
			CursorCoord.Y = BoardCoord.Y - 1;
		}

		if (CursorCoord.X < 0)
		{
			CursorCoord.X = 0;
		}
		else if (CursorCoord.X > BoardCoord.X - 1)
		{
			CursorCoord.X = BoardCoord.X - 1;
		}
	}
}

void Board::SetCell(int InRow, int InColume, Cell InData)
{
	if (BoardCoord.Y <= InRow || BoardCoord.X <= InColume)
	{
		return;
	}

	BoardMatrix[InRow][InColume] = InData;
}

Cell Board::GetCell(int InRow, int InColume)
{
	if (BoardCoord.Y <= InRow || 0 > InRow || BoardCoord.X <= InColume || 0 > InColume)
	{
		return Cell::Error;
	}

	return BoardMatrix[InRow][InColume];
}

void Board::InitBoard()
{
	BoardMatrix = new Cell * [BoardCoord.Y];
	for (int i = 0; i < BoardCoord.Y; ++i)
	{
		BoardMatrix[i] = new Cell[BoardCoord.X];

		for (int j = 0; j < BoardCoord.X; ++j)
		{
			BoardMatrix[i][j] = Cell::Empty;
		}
	}

	CellMap[Cell::Empty] = ' ';
	CellMap[Cell::Wall] = '#';
	CellMap[Cell::Path] = '!';

	FindMatrix = new bool * [BoardCoord.Y];
	for (int i = 0; i < BoardCoord.Y; ++i)
	{
		FindMatrix[i] = new bool[BoardCoord.X];

		for (int j = 0; j < BoardCoord.X; ++j)
		{
			FindMatrix[i][j] = false;
		}
	}

	PathMatrix = new Point * [BoardCoord.Y];
	ClearPathMatrix();
}

void Board::ClearBoard()
{
	for (int i = 0; i < BoardCoord.Y; ++i)
	{
		for (int j = 0; j < BoardCoord.X; ++j)
		{
			BoardMatrix[i][j] = Cell::Empty;
		}
	}

	for (int i = 0; i < BoardCoord.Y; ++i)
	{
		for (int j = 0; j < BoardCoord.X; ++j)
		{
			FindMatrix[i][j] = false;
		}
	}
}

bool Board::PathFind(int StartRow, int StartColume, int EndRow, int EndColume)
{
	ClearPathMatrix();
	FindMatrix[StartRow][StartColume] = true;
	PathQueue.Enqueue(Point(StartColume, StartRow));

	for (;PathQueue.IsEmpty() == false;)
	{
		int CurRow = PathQueue.GetData().Y;
		int CurColume = PathQueue.GetData().X;
		PathQueue.Dequeue();

		if (CurRow == EndRow && CurColume == EndColume)
		{
			return true;
		}

		if (CurRow - 1 >= 0 && FindMatrix[CurRow - 1][CurColume] == false && GetCell(CurRow - 1, CurColume) == Cell::Empty)
		{
			FindMatrix[CurRow - 1][CurColume] = true;
			PathMatrix[CurRow - 1][CurColume] = Point(CurColume, CurRow);
			PathQueue.Enqueue(Point(CurColume, CurRow - 1));
		}
		if (CurRow + 1 < BoardCoord.Y && FindMatrix[CurRow + 1][CurColume] == false && GetCell(CurRow + 1, CurColume) == Cell::Empty)
		{
			FindMatrix[CurRow + 1][CurColume] = true;
			PathMatrix[CurRow + 1][CurColume] = Point(CurColume, CurRow);
			PathQueue.Enqueue(Point(CurColume, CurRow + 1));
		}
		if (CurColume - 1 >= 0 && FindMatrix[CurRow][CurColume - 1] == false && GetCell(CurRow, CurColume - 1) == Cell::Empty)
		{
			FindMatrix[CurRow][CurColume - 1] = true;
			PathMatrix[CurRow][CurColume - 1] = Point(CurColume, CurRow);
			PathQueue.Enqueue(Point(CurColume - 1, CurRow));
		}
		if (CurColume + 1 < BoardCoord.X && FindMatrix[CurRow][CurColume + 1] == false && GetCell(CurRow, CurColume + 1) == Cell::Empty)
		{
			FindMatrix[CurRow][CurColume + 1] = true;
			PathMatrix[CurRow][CurColume + 1] = Point(CurColume, CurRow);
			PathQueue.Enqueue(Point(CurColume + 1, CurRow));
		}
	}

	return false;
}

void Board::AddPath(int EndRow, int EndColume)
{
	int CurRow = EndRow;
	int CurColume = EndColume;

	for (;;)
	{
		if (CurRow < 0 || CurColume < 0)
		{
			return;
		}

		BoardMatrix[CurRow][CurColume] = Cell::Path;
		int TempRow = CurRow;
		CurRow = PathMatrix[CurRow][CurColume].Y;
		CurColume = PathMatrix[TempRow][CurColume].X;
	}
}

void Board::ClearPathMatrix()
{
	if (PathMatrix == nullptr)
	{
		return;
	}

	for (int i = 0; i < BoardCoord.Y; ++i)
	{
		PathMatrix[i] = new Point[BoardCoord.X];

		for (int j = 0; j < BoardCoord.X; ++j)
		{
			PathMatrix[i][j] = Point(-1, -1);
		}
	}
}
