
#include <iostream>

#include "MainMenu.h"

void MainMenu::Activate()
{
	std::cout << "1. ���ӽ���" << std::endl;
	std::cout << "2. ��������" << std::endl;
	std::cout << "3. ��Ʈ�� �޴�" << std::endl;
	std::cout << "4. ���� �޴�" << std::endl;
	std::cout << "5. �����ϱ�" << std::endl;
}

EMenu MainMenu::Select(uint32 input)
{
	switch (input)
	{
	case 1:
		std::cout << "������ �����մϴ�." << std::endl;
		break;
	case 2:
		std::cout << "������ ���̺��մϴ�." << std::endl;
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
