#include "Map.h"
#include "Cell.h"
#include <stdlib.h>
#include <time.h>

Map::Map()
{
	CurrentMap = nullptr;
	MaxRow = 0;
	MaxColume = 0;
}

Map::~Map()
{
}

void Map::GenerateMap(int InRow, int InColume)
{
	MaxRow = InRow;
	MaxColume = InColume;

	CurrentMap = new Cell*[MaxRow];

	for (int i = 0; i < MaxRow; ++i)
	{
		CurrentMap[i] = new Cell[MaxColume];

		for (int j = 0; j < MaxColume; ++j)
		{
			CurrentMap[i][j].SetCurrentRow(i);
			CurrentMap[i][j].SetCurrentColume(j);
		}
	}
}

void Map::ReleaseMap()
{
	if (CurrentMap != nullptr)
	{
		for (int i = 0; i < MaxRow; ++i)
		{
			delete[] CurrentMap[i];
			CurrentMap[i] = nullptr;
		}

		delete[] CurrentMap;
		CurrentMap = nullptr;
	}
}

void Map::GenerateMine(int InMineNum)
{
	if (InMineNum >= MaxColume * MaxRow)
	{
		return;
	}

	if (CurrentMap == nullptr)
	{
		return;
	}

	srand(static_cast<unsigned int>(time(nullptr)));

	for (int i = 0; i < InMineNum; ++i)
	{
		int RandRow = rand() % MaxRow;
		int RandColume = rand() % MaxColume;

		if (CurrentMap[RandRow][RandColume].GetIsMine() == false)
		{
			int UpOffset = 1;
			int DownOffset = 1;
			int LeftOffset = 1;
			int RightOffset = 1;

			if (RandRow == 0)
			{
				LeftOffset = 0;
			}
			if (RandRow >= MaxRow - 1)
			{
				RightOffset = 0;
			}
			if (RandColume == 0)
			{
				UpOffset = 0;
			}
			if (RandColume >= MaxColume - 1)
			{
				DownOffset = 0;
			}

			CurrentMap[RandRow][RandColume].SetIsMine(true);
			CurrentMap[RandRow + RightOffset][RandColume + DownOffset].AddNearMine(RightOffset * DownOffset);
			CurrentMap[RandRow + RightOffset][RandColume].AddNearMine(RightOffset);
			CurrentMap[RandRow + RightOffset][RandColume - UpOffset].AddNearMine(RightOffset * UpOffset);
			CurrentMap[RandRow - LeftOffset][RandColume + DownOffset].AddNearMine(LeftOffset * DownOffset);
			CurrentMap[RandRow - LeftOffset][RandColume].AddNearMine(LeftOffset);
			CurrentMap[RandRow - LeftOffset][RandColume - UpOffset].AddNearMine(LeftOffset * UpOffset);
			CurrentMap[RandRow][RandColume + DownOffset].AddNearMine(DownOffset);
			CurrentMap[RandRow][RandColume - UpOffset].AddNearMine(UpOffset);
		}
		else
		{
			--i;
			continue;
		}
	}

}

Cell* Map::GetCell(int InRow, int InColume)
{
	if (InRow > MaxRow || InColume > MaxColume)
	{
		return nullptr;
	}

	return &CurrentMap[InRow][InColume];
}

Cell * Map::OpenCell(int InRow, int InColume)
{
	if (InRow > MaxRow || InColume > MaxColume)
	{
		return nullptr;
	}

	CurrentMap[InRow][InColume].SetIsOpen(true);

	return &CurrentMap[InRow][InColume];
}
