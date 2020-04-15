#pragma once

#include "IMenu.h"

class MainMenu final : public IMenu
{
public:

	MainMenu() = default;
	~MainMenu() = default;


	void Activate() override;
	EMenu Select(uint32 input) override;

private:
	
};
