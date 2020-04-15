#pragma once

#include "TypeDef.h"
#include "IMenu.h"


class ControlMenu final : public IMenu
{
public:

	ControlMenu() = default;
	virtual ~ControlMenu() = default;


	void Activate() override;
	EMenu Select(uint32 input) override;

private:

};