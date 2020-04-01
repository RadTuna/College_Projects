#include <iostream>
#include "Button.h"
#include "Motor.h"
#include "IceDispenser.h"

void Button::PressButton()
{
	Dispenser->RefreshButtonState();

	switch (State)
	{
	case ButtonState::Water:
		std::cout << "물 버튼을 눌렀습니다." << std::endl;
		break;
	case ButtonState::Slush:
		std::cout << "슬러시 버튼을 눌렀습니다." << std::endl;
		break;
	case ButtonState::Ice:
		std::cout << "얼음 버튼을 눌렀습니다." << std::endl;
		break;
	case ButtonState::Disable:
		std::cout << "돈이 부족합니다." << std::endl;
		return;
	default:
		break;
	}

	Dispenser->SetMoney(Dispenser->GetMoney() - Price);
	Dispenser->GetMainMotor()->Run(State);
}

void Button::SetState(ButtonState inState)
{
	State = inState;
}

void Button::SetPrice(int inPrice)
{
	Price = inPrice;
}

void Button::SetDispenser(IceDispenser* inDispenser)
{
	if (inDispenser != nullptr)
	{
		Dispenser = inDispenser;
	}
}