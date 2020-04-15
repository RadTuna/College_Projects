
#include <iostream>

#include "SoundMenu.h"

void SoundMenu::Activate()
{
	std::cout << "1. 마스터 볼륨" << std::endl;
	std::cout << "2. 음악 볼륨" << std::endl;
	std::cout << "3. 이펙트 볼륨" << std::endl;
	std::cout << "4. 라디오 볼륨" << std::endl;
	std::cout << "5. 뒤로가기" << std::endl;
}

EMenu SoundMenu::Select(uint32 input)
{
	switch (input)
	{
	case 1:
		std::cout << "마스터 볼륨을 조절합니다." << std::endl;
		break;
	case 2:
		std::cout << "음악 볼륨을 조절합니다." << std::endl;
		break;
	case 3:
		std::cout << "이펙트 볼륨을 조절합니다." << std::endl;
		break;
	case 4:
		std::cout << "라디오 볼륨을 조절합니다." << std::endl;
		break;
	case 5:
		return EMenu::Back;
	default:
		return EMenu::Unknown;
	}

	return EMenu::Unknown;
}

