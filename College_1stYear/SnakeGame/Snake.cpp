#include "Snake.h"

Snake::Snake(int InitPosX, int InitPosY)
	: SnakeCoords(10)
{
	SnakeHeading = Heading::RIGHT;
	SnakeCoords.Insert(0, Point(InitPosX, InitPosY));
}

Snake::~Snake()
{
}

void Snake::UpdateTail(const Point & NewCoord)
{
	SnakeCoords.Insert(0, NewCoord);
	SnakeCoords.Remove(SnakeCoords.GetLenght() - 1);
}

void Snake::UpdateTail(int CoordX, int CoordY)
{
	Point NewCoord(CoordX, CoordY);
	SnakeCoords.Insert(0, NewCoord);
	SnakeCoords.Remove(SnakeCoords.GetLenght() - 1);
}

void Snake::AddTail(const Point & NewCoord)
{
	SnakeCoords.Insert(0, NewCoord);
}

void Snake::AddTail(int CoordX, int CoordY)
{
	Point NewCoord(CoordX, CoordY);
	SnakeCoords.Insert(0, NewCoord);
}

void Snake::SetSnakeHeading(Heading InHeading)
{
	SnakeHeading = InHeading;
}
