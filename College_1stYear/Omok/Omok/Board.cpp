#include "Board.h"

Board::Board(int InBoardSize)
	: BoardSize(InBoardSize)
{
	GameBoard = new EBoard*[InBoardSize];
	for (int i = 0; i < InBoardSize; ++i)
	{
		GameBoard[i] = new EBoard[InBoardSize];

		for (int j = 0; j < InBoardSize; ++j)
		{
			GameBoard[i][j] = EBoard::Empty;
		}
	}
}

Board::~Board()
{
	if (GameBoard != nullptr)
	{
		for (int i = 0; i < BoardSize; ++i)
		{
			if (GameBoard[i] != nullptr)
			{
				delete[] GameBoard[i];
				GameBoard[i] = nullptr;
			}
		}

		delete[] GameBoard;
		GameBoard = nullptr;
	}
}

void Board::SetBoardElemenet(int Row, int Colume, EBoard InsertValue)
{
	GameBoard[Row][Colume] = InsertValue;
}

EBoard Board::GetBoardElement(int Row, int Colume) const
{
	if (Row >= BoardSize || Colume >= BoardSize || Row < 0 || Colume < 0)
	{
		return EBoard::InValid;
	}

	return GameBoard[Row][Colume];
}
