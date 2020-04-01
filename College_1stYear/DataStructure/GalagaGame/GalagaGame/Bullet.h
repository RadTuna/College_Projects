#pragma once
#include "Entity.h"
class Bullet : public Entity
{
public:

	explicit Bullet();
	explicit Bullet(int InitPosX, int InitPosY);
	explicit Bullet(const Point& InitPos);
	~Bullet() = default;

};

