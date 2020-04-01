#include <iostream>
#include "GameManager.h"

int main()
{
	GameManager Manager(10, 10, 2000, 3000);

	Manager.Awake();

	while (Manager.GetIsGameEnd() == false)
	{
		Manager.Update();
	}

	Manager.End();

	return 0;
}