#include <iostream>
#include "Motor.h"
#include "Button.h"

void Motor::Run(enum class ButtonState inState)
{
	switch (inState)
	{
	case ButtonState::Water:
		std::cout << "물이 나왔습니다." << std::endl;
		break;
	case ButtonState::Slush:
		std::cout << "슬러시가 나왔습니다." << std::endl;
		break;
	case ButtonState::Ice:
		std::cout << "얼음이 나왔습니다." << std::endl;
		break;
	default:
		break;
	}
}
