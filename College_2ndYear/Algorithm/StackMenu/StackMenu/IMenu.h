#pragma once

#include "TypeDef.h"

class IMenu
{
public:

	IMenu() = default;
	virtual ~IMenu() = default;

	virtual void Activate() = 0;
	virtual EMenu Select(uint32 input) = 0;
	
};
