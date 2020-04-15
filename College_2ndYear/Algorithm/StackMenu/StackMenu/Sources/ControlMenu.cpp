
#include <iostream>

#include "ControlMenu.h"

void ControlMenu::Activate()
{
	std::cout << "1. �̵� ����" << std::endl;
	std::cout << "2. ���� ����" << std::endl;
	std::cout << "3. �װ��� ����" << std::endl;
	std::cout << "4. �ڷ� ����" << std::endl;
}

EMenu ControlMenu::Select(uint32 input)
{
	switch (input)
	{
	case 1:
		std::cout << "�̵� ������ �����մϴ�." << std::endl;
		break;
	case 2:
		std::cout << "���� ������ �����մϴ�." << std::endl;
		break;
	case 3:
		std::cout << "�װ��� ������ �����մϴ�." << std::endl;
		break;
	case 4:
		return EMenu::Back;
	default:
		return EMenu::Unknown;
	}

	return EMenu::Unknown;
}

