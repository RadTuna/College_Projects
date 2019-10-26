#include "AccountHandler.h"
#include "Account.h"
#include <iostream>

AccountHandler::AccountHandler()
	: DataSize(32)
{
	AccountData = new Account[DataSize];
}

AccountHandler::AccountHandler(int AccountReserve)
	: DataSize(AccountReserve)
{
	AccountData = new Account[DataSize];
}

AccountHandler::~AccountHandler()
{
	if (AccountData != nullptr)
	{
		delete[] AccountData;
		AccountData = nullptr;
	}
}

bool AccountHandler::NewAccount(int AccountNumber, const char* InName, unsigned int InMoney)
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return false;
	}
	if (IsEmptyAccount(AccountNumber) == false)
	{
		return false;
	}

	AccountData[AccountNumber].SetAccount(AccountNumber, InName, InMoney);

	return true;
}

bool AccountHandler::DespoitInAccount(int AccountNumber, unsigned int InMoney)
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return false;
	}
	if (IsEmptyAccount(AccountNumber) == true)
	{
		return false;
	}

	Account* CurAccount = &AccountData[AccountNumber];

	CurAccount->SetCurrentMoney(CurAccount->GetCurrentMoney() + InMoney);

	return true;
}

bool AccountHandler::WithdrawInAccount(int AccountNumber, unsigned int InMoney)
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return false;
	}
	if (IsEmptyAccount(AccountNumber) == true)
	{
		return false;
	}

	if (AccountData[AccountNumber].GetCurrentMoney() < InMoney)
	{
		return false;
	}

	Account* CurAccount = &AccountData[AccountNumber];

	CurAccount->SetCurrentMoney(CurAccount->GetCurrentMoney() - InMoney);

	return true;
}

const char* AccountHandler::GetAccountName(int AccountNumber) const
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return nullptr;
	}
	if (IsEmptyAccount(AccountNumber) == true)
	{
		return nullptr;
	}

	return AccountData[AccountNumber].GetName();
}

unsigned int AccountHandler::GetAccountMoney(int AccountNumber) const
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return 0;
	}
	if (IsEmptyAccount(AccountNumber) == true)
	{
		return 0;
	}

	return AccountData[AccountNumber].GetCurrentMoney();
}

bool AccountHandler::GetAccountData(int AccountNumber, char*& OutName, unsigned int& OutMoney)
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return false;
	}
	if (IsEmptyAccount(AccountNumber) == true)
	{
		return false;
	}

	OutName = const_cast<char*>(AccountData[AccountNumber].GetName());
	OutMoney = AccountData[AccountNumber].GetCurrentMoney();

	return true;
}

bool AccountHandler::IsValidAccount(int AccountNumber) const
{
	return AccountNumber < DataSize;
}

bool AccountHandler::IsEmptyAccount(int AccountNumber) const
{
	return AccountData[AccountNumber].GetAccountNumber() == -1;
}
