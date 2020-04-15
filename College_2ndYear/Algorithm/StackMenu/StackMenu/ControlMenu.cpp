
#include <iostream>

#include "ControlMenu.h"

void ControlMenu::Activate()
{
	std::cout << "1. 이동 조작" << std::endl;
	std::cout << "2. 전투 조작" << std::endl;
	std::cout << "3. 항공기 조작" << std::endl;
	std::cout << "4. 뒤로 가기" << std::endl;
}

EMenu ControlMenu::Select(uint32 input)
{
	switch (input)
	{
	case 1:
		std::cout << "이동 조작을 수정합니다." << std::endl;
		break;
	case 2:
		std::cout << "전투 조작을 수정합니다." << std::endl;
		break;
	case 3:
		std::cout << "항공기 조작을 수정합니다." << std::endl;
		break;
	case 4:
		return EMenu::Back;
	default:
		return EMenu::Unknown;
	}

	return EMenu::Unknown;
}

