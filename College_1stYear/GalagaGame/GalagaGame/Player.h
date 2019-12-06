#pragma once
#include "Entity.h"
class Player : public Entity
{
public:

	explicit Player();
	explicit Player(int InitPosX, int InitPosY);
	explicit Player(const Point& InitPos);
	~Player() = default;

	void MoveLeft(int Offset = 1, int limit = 0);
	void MoveRight(int Offset = 1, int limit = 100);

};

