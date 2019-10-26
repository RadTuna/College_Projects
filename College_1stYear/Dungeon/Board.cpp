#include "Board.h"

Board::Board(int InRow, int InColume)
	: MaxRow(InRow),
	MaxColume(InColume)
{
	BoardData = new Tile*[MaxRow];

	for (int i = 0; i < MaxRow; ++i)
	{
		BoardData[i] = new Tile[MaxColume];

		for (int j = 0; j < MaxColume; ++j)
		{
			BoardData[i][j] = Tile::Empty;
		}
	}
}

Board::~Board()
{
	if (BoardData != nullptr)
	{
		for (int i = 0; i < MaxRow; ++i)
		{
			if (BoardData != nullptr)
			{
				delete[] BoardData[i];
				BoardData[i] = nullptr;
			}
		}

		delete[] BoardData;
		BoardData = nullptr;
	}

}

void Board::SetTile(int Row, int Colume, Tile InData)
{
	BoardData[Row][Colume] = InData;
}

Tile Board::GetTile(int Row, int Colume)
{
	return BoardData[Row][Colume];
}
