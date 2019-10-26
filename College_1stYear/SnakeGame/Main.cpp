#include <iostream>
#include "GameManager.h"

int main()
{
	GameManager Manager(10, 10, 500, 4000, 5, 5);

	Manager.Awake();

	while (Manager.GetIsGameEnd() == false)
	{
		Manager.Update();
	} 

	Manager.End();

	std::cout << "게임종료!" << std::endl;

	return 0;
}