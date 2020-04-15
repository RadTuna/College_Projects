#pragma once

#include "TypeDef.h"
#include "Stack.h"

class MenuHandler final
{
public:

	MenuHandler() = default;
	~MenuHandler();

	void Run();

private:

	Stack<class IMenu*> mMenus;
	
};

