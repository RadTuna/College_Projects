#pragma once

#include <memory>
#include "Entity.h"

class Board
{
public:

	explicit Board(int InRow, int InColume);
	~Board() = default;

	void SetTile(int Row, int Colume, const Entity& InData);
	const Entity& GetTile(int Row, int Colume);

private:

	std::unique_ptr<Entity[]> BoardData;
	int MaxRow;
	int MaxColume;

public:

	__forceinline const int GetMaxRow() const { return MaxRow; }
	__forceinline const int GetMaxColume() const { return MaxColume; }

};

