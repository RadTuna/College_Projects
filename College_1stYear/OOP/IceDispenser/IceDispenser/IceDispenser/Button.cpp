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
		std::cout << "�� ��ư�� �������ϴ�." << std::endl;
		break;
	case ButtonState::Slush:
		std::cout << "������ ��ư�� �������ϴ�." << std::endl;
		break;
	case ButtonState::Ice:
		std::cout << "���� ��ư�� �������ϴ�." << std::endl;
		break;
	case ButtonState::Disable:
		std::cout << "���� �����մϴ�." << std::endl;
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