#include <iostream>
#include "GameManager.h"

int main()
{
	GameManager Manager(10, 10, 1000, 300, 5, 9);

	Manager.Awake();

	while (Manager.GetIsGameEnd() == false)
	{
		Manager.Update();
	} 

	Manager.End();

	std::cout << "��������!" << std::endl;

	return 0;
}