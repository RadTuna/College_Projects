#pragma once

#include "List.h"

enum class Heading
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Snake
{
public:

	Snake(int InitPosX, int InitPosY);
	~Snake();

	void UpdateTail(const Point& NewCoord);
	void UpdateTail(int CoordX, int CoordY);
	void AddTail(const Point& NewCoord);
	void AddTail(int CoordX, int CoordY);
	void SetSnakeHeading(Heading InHeading);

private:

	Heading SnakeHeading;
	List SnakeCoords;

public:

	__forceinline List& GetSnakeList() const { return (List&)SnakeCoords; }
	__forceinline Heading GetSnakeHeading() const { return SnakeHeading; }

};
