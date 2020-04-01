#pragma once

#include "PlayerBase.h"

class NonPlayer : public PlayerBase
{
public:

	NonPlayer();
	~NonPlayer() = default;

	virtual void InputNumber() override;

};

