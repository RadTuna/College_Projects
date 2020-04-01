#include "AccountHandler.h"
#include "Account.h"
#include "HighCreditAccount.h"
#include "NormalAccount.h"
#include <iostream>

AccountHandler::AccountHandler()
	: DataSize(32),
	AccountData(nullptr)
{
	AccountData = new Account*[DataSize];

	for (int i = 0; i < DataSize; ++i)
	{
		AccountData[i] = nullptr;
	}
}

AccountHandler::AccountHandler(int AccountReserve)
	: DataSize(AccountReserve)
{
	AccountData = new Account*[DataSize];

	for (int i = 0; i < DataSize; ++i)
	{
		AccountData[i] = nullptr;
	}
}

AccountHandler::~AccountHandler()
{
	if (AccountData != nullptr)
	{
		for (int i = 0; i < DataSize; ++i)
		{
			if (AccountData[i] != nullptr)
			{
				delete AccountData[i];
				AccountData[i] = nullptr;
			}
		}
		delete[] AccountData;
		AccountData = nullptr;
	}
}

bool AccountHandler::NewNormalAccount(int AccountNumber, const char* InName, unsigned int InMoney)
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return false;
	}
	if (IsEmptyAccount(AccountNumber) == false)
	{
		return false;
	}

	AccountData[AccountNumber] = new NormalAccount(3);

	AccountData[AccountNumber]->SetAccount(AccountNumber, InName, InMoney, 'F');

	return true;
}

bool AccountHandler::NewHighCreditAccount(int AccountNumber, const char* InName, unsigned int InMoney, char CreditRating)
{
	if (IsValidAccount(AccountNumber) == false)
	{
		return false;
	}
	if (IsEmptyAccount(AccountNumber) == false)
	{
		return false;
	}

	AccountData[AccountNumber] = new HighCreditAccount(3);
	
	AccountData[AccountNumber]->SetAccount(AccountNumber, InName, InMoney, CreditRating);

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

	Account* CurAccount = AccountData[AccountNumber];

	CurAccount->DepositMoney(InMoney);

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

	if (AccountData[AccountNumber]->GetCurrentMoney() < InMoney)
	{
		return false;
	}

	Account* CurAccount = AccountData[AccountNumber];

	CurAccount->WithDrawMoney(InMoney);

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

	return AccountData[AccountNumber]->GetName();
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

	return AccountData[AccountNumber]->GetCurrentMoney();
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

	OutName = const_cast<char*>(AccountData[AccountNumber]->GetName());
	OutMoney = AccountData[AccountNumber]->GetCurrentMoney();

	return true;
}

bool AccountHandler::IsValidAccount(int AccountNumber) const
{
	return AccountNumber < DataSize;
}

bool AccountHandler::IsEmptyAccount(int AccountNumber) const
{
	if (AccountData[AccountNumber] == nullptr)
	{
		return true;
	}

	return false;
}
