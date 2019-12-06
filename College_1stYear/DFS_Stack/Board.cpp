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
				AddPath();
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
	if (BoardCoord.Y <= InRow || BoardCoord.X <= InColume)
	{
		return Cell::Empty;
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
	if (StartRow < 0 || StartRow > BoardCoord.Y - 1 || StartColume < 0 || StartColume > BoardCoord.X - 1)
	{
		return false;
	}
	if (GetCell(StartRow, StartColume) != Cell::Empty || GetCell(EndRow, EndColume) != Cell::Empty)
	{
		return false;
	}
	if (StartRow == EndRow && StartColume == EndColume)
	{
		return true;
	}
	if (FindMatrix[StartRow][StartColume] == true)
	{
		return false;
	}

	FindMatrix[StartRow][StartColume] = true;
	PathStack.Push(Point(StartColume, StartRow));

	bool Result = false;
	Result = PathFind(StartRow - 1, StartColume, EndRow, EndColume);
	if (Result)
	{
		return true;
	}
	Result = PathFind(StartRow, StartColume + 1, EndRow, EndColume);
	if (Result)
	{
		return true;
	}
	Result = PathFind(StartRow + 1, StartColume, EndRow, EndColume);
	if (Result)
	{
		return true;
	}
	Result = PathFind(StartRow, StartColume - 1, EndRow, EndColume);
	if (Result)
	{
		return true;
	}

	PathStack.Pop();
	return false;
}

void Board::AddPath()
{
	for (; PathStack.IsEmpty() == false;)
	{
		Point CurPoint = PathStack.Get();
		BoardMatrix[CurPoint.Y][CurPoint.X] = Cell::Path;
		PathStack.Pop();
	}
}
