#include "Player.h"

Player::Player()
	: Entity()
{
	SetTag("Player");
}

Player::Player(int InitPosX, int InitPosY)
	: Entity(InitPosX, InitPosY)
{
	SetTag("Player");
}

Player::Player(const Point& InitPos)
	: Entity(InitPos.GetPointX(), InitPos.GetPointY())
{
	SetTag("Player");
}

void Player::MoveLeft(int Offset, int limit)
{
	if (GetPosX() <= limit)
	{
		return;
	}

	SetPosition(GetPosX() - Offset, GetPosY());
}

void Player::MoveRight(int Offset, int limit)
{
	if (GetPosX() >= limit)
	{
		return;
	}

	SetPosition(GetPosX() + Offset , GetPosY());
}
