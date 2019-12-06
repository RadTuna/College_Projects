#include "Enemy.h"

Enemy::Enemy() 
	: Entity()
{
	SetTag("Enemy");
}

Enemy::Enemy(int InitPosX, int InitPosY)
	: Entity(InitPosX, InitPosY)
{
	SetTag("Enemy");
}

Enemy::Enemy(const Point& InitPos)
	: Entity(InitPos.GetPointX(), InitPos.GetPointY())
{
	SetTag("Enemy");
}

