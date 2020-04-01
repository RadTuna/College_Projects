#include "NormalAccount.h"

NormalAccount::NormalAccount(int InInterest)
	: Interest(InInterest)
{
}

NormalAccount::~NormalAccount()
{
}

void NormalAccount::SetAccount(int InAccNum, const char* InName, unsigned int InMoney, char CreditRating)
{
	Account::SetAccount(InAccNum, InName, InMoney, CreditRating);
}

unsigned int NormalAccount::DepositMoney(unsigned int InMoney)
{
	InMoney += static_cast<unsigned int>(static_cast<float>(InMoney) * 0.01f * Interest);
	CurrentMoney += InMoney;
	return CurrentMoney;
}