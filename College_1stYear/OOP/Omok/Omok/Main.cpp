#include <iostream>
#include "GameManager.h"

using namespace std;

int main()
{
	constexpr int BoardSize = 10;

	GameManger Manager(BoardSize);

	Manager.Awake();
	while (Manager.IsOmok() == false)
	{
		Manager.Update();
	}
	Manager.Destroy();

	return 0;
}