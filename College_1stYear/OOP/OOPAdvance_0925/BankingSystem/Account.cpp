#include "Account.h"
#include <string.h>

Account::Account()
	: AccountNumber(0),
	Name(nullptr),
	CurrentMoney(0)
{
}

Account::Account(int InAccNum, const char* InName, unsigned int InMoney)
	: AccountNumber(InAccNum),
	CurrentMoney(InMoney)
{
	Name = new char[strlen(InName) + 1];
	strcpy_s(Name, strlen(InName) + 1, InName);
}

Account::~Account()
{
	if (Name != nullptr)
	{
		delete Name;
		Name = nullptr;
	}
}

void Account::SetAccount(int InAccNum, const char* InName, unsigned int InMoney)
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

void Account::SetCurrentMoney(int InMoeny)
{
	CurrentMoney = InMoeny;
}
