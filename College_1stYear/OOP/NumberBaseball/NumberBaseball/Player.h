#pragma once

#include "PlayerBase.h"

class Player : public PlayerBase
{
public:

	Player() = default;
	~Player() = default;

	virtual void InputNumber() override;

};

