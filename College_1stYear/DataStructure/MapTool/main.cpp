#include "util.h"
#include "Board.h"
#include <conio.h>

int main()
{
	constexpr int BoardRow = 33;
	constexpr int BoardColume = 20;

	Board Map(BoardRow, BoardColume);

	ScreenInit();

	while (true)
	{
		ScreenClear();

		Map.UserInput();
		Map.DrawBoard();

		ScreenFlipping();
	}

	_getch();

	ScreenRelease();

	return 0;

}