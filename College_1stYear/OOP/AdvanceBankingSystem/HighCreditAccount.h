#pragma once
#include "Account.h"
class HighCreditAccount : public Account
{
public:

	HighCreditAccount(int InInterest);
	virtual ~HighCreditAccount();

	virtual void SetAccount(int InAccNum, const char* InName, unsigned int InMoney, char CreditRating) override;
	virtual unsigned int DepositMoney(unsigned int InMoney) override;

private:

	int Interest;
	int BonusInterest;

};

