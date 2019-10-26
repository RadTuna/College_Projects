#pragma once

enum class Tile
{
	Empty,
	Apple,
};


class Board
{
public:

	Board(int InRow, int InColume);
	~Board();

	void SetTile(int Row, int Colume, Tile InData);
	Tile GetTile(int Row, int Colume);

private:

	Tile** BoardData;
	int MaxRow;
	int MaxColume;

public:

	__forceinline const int GetMaxRow() const { return MaxRow; }
	__forceinline const int GetMaxColume() const { return MaxColume; }

};

