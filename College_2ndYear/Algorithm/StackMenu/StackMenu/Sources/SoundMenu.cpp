
#include <iostream>

#include "SoundMenu.h"

void SoundMenu::Activate()
{
	std::cout << "1. ������ ����" << std::endl;
	std::cout << "2. ���� ����" << std::endl;
	std::cout << "3. ����Ʈ ����" << std::endl;
	std::cout << "4. ���� ����" << std::endl;
	std::cout << "5. �ڷΰ���" << std::endl;
}

EMenu SoundMenu::Select(uint32 input)
{
	switch (input)
	{
	case 1:
		std::cout << "������ ������ �����մϴ�." << std::endl;
		break;
	case 2:
		std::cout << "���� ������ �����մϴ�." << std::endl;
		break;
	case 3:
		std::cout << "����Ʈ ������ �����մϴ�." << std::endl;
		break;
	case 4:
		std::cout << "���� ������ �����մϴ�." << std::endl;
		break;
	case 5:
		return EMenu::Back;
	default:
		return EMenu::Unknown;
	}

	return EMenu::Unknown;
}

