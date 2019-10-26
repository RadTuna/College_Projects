#include "Board.h"
#include "util.h"
#include <conio.h>

Board::Board()
	: BoardRow(33),
	BoardColume(20),
	CurRow(0),
	CurColume(0),
	BoardMatrix(nullptr)

{
	InitBoard();
}

Board::Board(int InRow, int InColume)
	: BoardRow(InRow),
	BoardColume(InColume),
	CurRow(0),
	CurColume(0),
	BoardMatrix(nullptr)
{
	InitBoard();
}

Board::~Board()
{
	if (BoardMatrix != nullptr)
	{
		for (int i = 0; i < BoardRow; ++i)
		{
			if (BoardMatrix[i] != nullptr)
			{
				delete[] BoardMatrix[i];
				BoardMatrix = nullptr;
			}
		}

		delete[] BoardMatrix;
		BoardMatrix = nullptr;
	}
}

void Board::DrawBoard()
{
	for (int i = 0; i < BoardRow; ++i)
	{
		for (int j = 0; j < BoardColume; ++j)
		{
			std::map<Cell, char>::iterator iter = CellMap.find(GetCell(i, j));
			if (iter == CellMap.end())
			{
				continue;
			}

			char CharBuf[2];
			CharBuf[0] = iter->second;
			CharBuf[1] = '\0';
			ScreenPrint(i * 2, j, CharBuf);
		}
	}

	ScreenPrint(CurRow * 2, CurColume, const_cast<char*>("^"));
}

void Board::UserInput()
{
	if (_kbhit())
	{
		int CurInput = _getch();

		switch (CurInput)
		{
		case LEFT:
			CurRow--;
			break;
		case RIGHT:
			CurRow++;
			break;
		case UP:
			CurColume--;
			break;
		case DOWN:
			CurColume++;
			break;
		case ONE:
			SetCell(CurRow, CurColume, Cell::Empty);
			break;
		case TWO:
			SetCell(CurRow, CurColume, Cell::Grass);
			break;
		case THREE:
			SetCell(CurRow, CurColume, Cell::Water);
			break;
		case FOUR:
			SetCell(CurRow, CurColume, Cell::Wall);
			break;
		}

		if (CurRow < 0)
		{
			CurRow = 0;
		}
		else if (CurRow > BoardRow - 1)
		{
			CurRow = BoardRow - 1;
		}

		if (CurColume < 0)
		{
			CurColume = 0;
		}
		else if (CurColume > BoardColume - 1)
		{
			CurColume = BoardColume - 1;
		}
	}
}

void Board::SetCell(int InRow, int InColume, Cell InData)
{
	if (BoardRow <= InRow || BoardColume <= InColume)
	{
		return;
	}

	BoardMatrix[InRow][InColume] = InData;
}

Cell Board::GetCell(int InRow, int InColume)
{
	if (BoardRow <= InRow || BoardColume <= InColume)
	{
		return Cell::Empty;
	}

	return BoardMatrix[InRow][InColume];
}

void Board::InitBoard()
{
	BoardMatrix = new Cell * [BoardRow];
	for (int i = 0; i < BoardRow; ++i)
	{
		BoardMatrix[i] = new Cell[BoardColume];

		for (int j = 0; j < BoardColume; ++j)
		{
			BoardMatrix[i][j] = Cell::Empty;
		}
	}

	CellMap[Cell::Empty] = ' ';
	CellMap[Cell::Grass] = '@';
	CellMap[Cell::Water] = '*';
	CellMap[Cell::Wall] = '#';
}
