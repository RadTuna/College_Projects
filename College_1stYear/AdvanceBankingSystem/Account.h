#pragma once

class Account
{
public:

	Account();
	Account(int InAccNum, const char* InName, unsigned int InMoney);
	~Account();

	void SetAccount(int InAccNum, const char* InName, unsigned int InMoney);
	void SetCurrentMoney(int InMoeny);

private:

	int AccountNumber;
	char* Name;
	unsigned int CurrentMoney;

public:

	inline int GetAccountNumber() const { return AccountNumber; }
	inline unsigned int GetCurrentMoney() const { return CurrentMoney; }
	inline const char* GetName() const { return Name; }

};
