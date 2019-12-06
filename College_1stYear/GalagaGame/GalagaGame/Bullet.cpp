#include "Bullet.h"

Bullet::Bullet()
	: Entity()
{
	SetTag("Bullet");
}

Bullet::Bullet(int InitPosX, int InitPosY)
	: Entity(InitPosX, InitPosY)
{
	SetTag("Bullet");
}

Bullet::Bullet(const Point& InitPos)
	: Entity(InitPos.GetPointX(), InitPos.GetPointY())
{
	SetTag("Bullet");
}

