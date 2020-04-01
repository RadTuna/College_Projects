#pragma once
#include "Account.h"
class NormalAccount : public Account
{
public:

	NormalAccount(int InInterest);
	virtual ~NormalAccount();

	virtual void SetAccount(int InAccNum, const char* InName, unsigned int InMoney, char CreditRating) override;
	virtual unsigned int DepositMoney(unsigned int InMoney) override;

private:

	int Interest;

};

