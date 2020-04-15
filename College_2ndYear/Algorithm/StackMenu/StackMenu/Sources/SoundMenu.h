#pragma once

#include "TypeDef.h"
#include "IMenu.h"


class SoundMenu final : public IMenu
{
public:

	SoundMenu() = default;
	virtual ~SoundMenu() = default;


	void Activate() override;
	EMenu Select(uint32 input) override;

private:
	
};
