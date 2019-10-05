#include "Player.h"
#include <iostream>

void Player::InputNumber()
{
	std::cout << "3자리의 숫자를 입력해주세요." << std::endl;

	char InString[10];

	std::cin >> InString;

	if (CheckNumber(atoi(InString)) == false)
	{
		InputNumber();
		return;
	}

	SetCurrentNumber(atoi(InString));
}
