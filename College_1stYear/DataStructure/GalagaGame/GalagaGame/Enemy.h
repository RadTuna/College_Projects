#pragma once
#include "Entity.h"
class Enemy : public Entity
{
public:

	explicit Enemy();
	explicit Enemy(int InitPosX, int InitPosY);
	explicit Enemy(const Point& InitPos);
	~Enemy() = default;

};

