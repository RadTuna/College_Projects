#include "IceDispenser.h"
#include "Button.h"

int main()
{
	IceDispenser* Dispenser = new IceDispenser;

	Dispenser->Initialize();

	Dispenser->InsertMoney(1000);
	Dispenser->GetSlushButton()->PressButton();
	Dispenser->GetWaterButton()->PressButton();
	Dispenser->GetIceButton()->PressButton();
	Dispenser->ReturnMoney();

	Dispenser->Release();

	if (Dispenser != nullptr)
	{
		delete Dispenser;
		Dispenser = nullptr;
	}

	return 0;
}