
#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool Result;

	System = new SystemClass;
	if (System == nullptr)
	{
		return 0;
	}

	Result = System->Initialize();
	if (Result == true)
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}