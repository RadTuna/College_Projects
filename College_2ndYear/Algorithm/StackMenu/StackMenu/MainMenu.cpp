
#include <iostream>

#include "MainMenu.h"

void MainMenu::Activate()
{
	std::cout << "1. 게임시작" << std::endl;
	std::cout << "2. 게임저장" << std::endl;
	std::cout << "3. 컨트롤 메뉴" << std::endl;
	std::cout << "4. 사운드 메뉴" << std::endl;
	std::cout << "5. 종료하기" << std::endl;
}

EMenu MainMenu::Select(uint32 input)
{
	switch (input)
	{
	case 1:
		std::cout << "게임을 시작합니다." << std::endl;
		break;
	case 2:
		std::cout << "게임을 세이브합니다." << std::endl;
		break;
	case 3:
		return EMenu::ControlMenu;
	case 4:
		return EMenu::SoundMenu;
	case 5:
		return EMenu::Back;
	default:
		return EMenu::Unknown;
	}

	return EMenu::Unknown;
}
