#include "Account.h"
#include <string.h>

Account::Account()
	: AccountNumber(-1),
	Name(nullptr),
	CurrentMoney(0)
{
}

Account::~Account()
{
	if (Name != nullptr)
	{
		delete Name;
		Name = nullptr;
	}
}

void Account::SetAccount(int InAccNum, const char* InName, unsigned int InMoney, char CreditRating)
{
	if (Name != nullptr)
	{
		delete[] Name;
		Name = nullptr;
	}

	Name = new char[strlen(InName) + 1];
	strcpy_s(Name, strlen(InName) + 1, InName);

	AccountNumber = InAccNum;
	CurrentMoney = InMoney;
}

unsigned int Account::WithDrawMoney(unsigned int InMoeny)
{
	CurrentMoney -= InMoeny;
	return CurrentMoney;
}

