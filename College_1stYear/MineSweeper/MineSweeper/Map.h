#pragma once

class Map
{
public:

	Map();
	~Map();

	void GenerateMap(int InRow, int InColume);
	void ReleaseMap();
	void GenerateMine(int InMineNum);
	class Cell* GetCell(int InRow, int InColume);
	class Cell* OpenCell(int InRow, int InColume);

private:

	class Cell** CurrentMap;
	int MaxRow;
	int MaxColume;

public:

	inline int GetMaxRow() const { return MaxRow; }
	inline int GetMaxColume() const { return MaxColume; }

};