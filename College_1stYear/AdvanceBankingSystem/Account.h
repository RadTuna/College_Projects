#pragma once

#include <string.h>

class Account
{
public:

	Account();
	virtual  ~Account();

	virtual void SetAccount(int InAccNum, const char* InName, unsigned int InMoney, char CreditRating);
	virtual unsigned int DepositMoney(unsigned int InMoney) = 0;
	unsigned int WithDrawMoney(unsigned int InMoeny);

protected:

	int AccountNumber;
	char* Name;
	unsigned int CurrentMoney;

public:

	inline int GetAccountNumber() const { return AccountNumber; }
	inline unsigned int GetCurrentMoney() const { return CurrentMoney; }
	inline const char* GetName() const { return Name; }

};
