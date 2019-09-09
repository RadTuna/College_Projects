#include <iostream>
#include "Motor.h"
#include "Button.h"

void Motor::Run(enum class ButtonState inState)
{
	switch (inState)
	{
	case ButtonState::Water:
		std::cout << "���� ���Խ��ϴ�." << std::endl;
		break;
	case ButtonState::Slush:
		std::cout << "�����ð� ���Խ��ϴ�." << std::endl;
		break;
	case ButtonState::Ice:
		std::cout << "������ ���Խ��ϴ�." << std::endl;
		break;
	default:
		break;
	}
}
