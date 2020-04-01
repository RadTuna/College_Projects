#include <iostream>
#include "IceDispenser.h"
#include "Button.h"
#include "Motor.h"

void IceDispenser::Initialize()
{
	MainMotor = new Motor;

	WaterButton = new Button;
	WaterButton->SetDispenser(this);
	WaterButton->SetState(ButtonState::Disable);
	WaterButton->SetPrice(100);

	SlushButton = new Button;
	SlushButton->SetDispenser(this);
	SlushButton->SetState(ButtonState::Disable);
	SlushButton->SetPrice(200);

	IceButton = new Button;
	IceButton->SetDispenser(this);
	IceButton->SetState(ButtonState::Disable);
	IceButton->SetPrice(300);

	CurrentMoney = 0;
}

void IceDispenser::Release()
{
	if (IceButton != nullptr)
	{
		delete IceButton;
		IceButton = nullptr;
	}

	if (SlushButton == nullptr)
	{
		delete SlushButton;
		SlushButton = nullptr;
	}

	if (IceButton == nullptr)
	{
		delete IceButton;
		IceButton = nullptr;
	}

	if (MainMotor == nullptr)
	{
		delete MainMotor;
		MainMotor = nullptr;
	}
}

void IceDispenser::InsertMoney(int inMoney)
{
	CurrentMoney += inMoney;

	std::cout << "현재 금액은" << CurrentMoney << "원 입니다." << std::endl;

	RefreshButtonState();
}

void IceDispenser::ReturnMoney()
{
	std::cout << "거스름돈은 " << CurrentMoney << "원 입니다." << std::endl;

	CurrentMoney = 0;
}

void IceDispenser::SetMoney(int inMoney)
{
	CurrentMoney = inMoney;
}

void IceDispenser::RefreshButtonState()
{
	WaterButton->SetState(ButtonState::Disable);
	SlushButton->SetState(ButtonState::Disable);
	IceButton->SetState(ButtonState::Disable);

	if (CurrentMoney > WaterButton->GetPrice())
	{
		WaterButton->SetState(ButtonState::Water);
	}
	if (CurrentMoney > SlushButton->GetPrice())
	{
		SlushButton->SetState(ButtonState::Slush);
	}
	if (CurrentMoney > IceButton->GetPrice())
	{
		IceButton->SetState(ButtonState::Ice);
	}
}
