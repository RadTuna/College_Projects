#include "Player.h"
#include <iostream>

void Player::InputNumber()
{
	std::cout << "3�ڸ��� ���ڸ� �Է����ּ���." << std::endl;

	char InString[10];

	std::cin >> InString;

	if (CheckNumber(atoi(InString)) == false)
	{
		InputNumber();
		return;
	}

	SetCurrentNumber(atoi(InString));
}
