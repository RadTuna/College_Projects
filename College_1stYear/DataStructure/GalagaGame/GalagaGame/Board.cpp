#include "Board.h"

Board::Board(int InRow, int InColume)
	: MaxRow(InRow),
	MaxColume(InColume)
{
	BoardData = std::make_unique<Entity[]>(MaxRow * MaxColume);
}

void Board::SetTile(int Row, int Colume, const Entity& InData)
{
	if (Row < 0 || Row >= MaxRow)
	{
		return;
	}
	if (Colume < 0 || Colume >= MaxColume)
	{
		return;
	}

	BoardData[(Row * MaxColume) + Colume] = InData;
}

const Entity& Board::GetTile(int Row, int Colume)
{
	return BoardData[(Row * MaxColume) + Colume];
}
