#pragma once

enum EBoard
{
	White,
	Black,
	Empty,
	InValid
};

class Board
{
public:

	Board(int InBoardSize);
	~Board();

	void SetBoardElemenet(int Row, int Colume, EBoard InsertValue);
	EBoard GetBoardElement(int Row, int Colume) const;

private:
	
	EBoard** GameBoard;
	int BoardSize;

public:
	inline int GetBoardSize() const { return BoardSize; }

};


