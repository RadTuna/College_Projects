#include "Cell.h"

Cell::Cell()
{
	IsOpen = false;
	IsMine = false;
	NearMine = 0;
	CurrentColume = 0;
	CurrentRow = 0;
}

Cell::~Cell()
{
}

void Cell::SetCurrentRow(int InRow)
{
	CurrentRow = InRow;
}

void Cell::SetCurrentColume(int InColume)
{
	CurrentColume = InColume;
}

void Cell::SetIsOpen(bool InOpen)
{
	IsOpen = InOpen;
}

void Cell::SetIsMine(bool InMine)
{
	IsMine = InMine;
}

void Cell::AddNearMine(int InAdd)
{
	NearMine += InAdd;
}
