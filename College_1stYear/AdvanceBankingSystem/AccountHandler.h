#pragma once

class AccountHandler
{
public:

	AccountHandler();
	AccountHandler(int AccountReserve);
	AccountHandler(const AccountHandler& other) = delete;
	~AccountHandler();

	bool NewAccount(int AccountNumber, const char* InName, unsigned int InMoney);
	bool DespoitInAccount(int AccountNumber, unsigned int InMoney);
	bool WithdrawInAccount(int AccountNumber, unsigned int InMoney);
	const char* GetAccountName(int AccountNumber) const;
	unsigned int GetAccountMoney(int AccountNumber) const;
	bool GetAccountData(int AccountNumber, char*& OutName, unsigned int& OutMoney);
	bool IsValidAccount(int AccountNumber) const;
	bool IsEmptyAccount(int AccountNumber) const;

private:

	class Account* AccountData;
	int DataSize;

public:

	inline int GetDataSize() const { return DataSize; }

};
