#include "HighCreditAccount.h"

HighCreditAccount::HighCreditAccount(int InInterest)
	: Interest(InInterest),
	BonusInterest(0)
{
}

HighCreditAccount::~HighCreditAccount()
{
}

void HighCreditAccount::SetAccount(int InAccNum, const char* InName, unsigned int InMoney, char CreditRating)
{
	Account::SetAccount(InAccNum, InName, InMoney, CreditRating);

	switch (CreditRating)
	{
	case 'A':
		BonusInterest = 7;
		break;
	case 'B':
		BonusInterest = 4;
		break;
	case 'C':
		BonusInterest = 2;
		break;
	}
}

unsigned int HighCreditAccount::DepositMoney(unsigned int InMoney)
{
	InMoney += static_cast<unsigned int>(static_cast<float>(InMoney) * 0.01f * (Interest + BonusInterest));
	CurrentMoney += InMoney;
	return CurrentMoney;
}

