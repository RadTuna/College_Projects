#include <iostream>
#include "Account.h"

int main()
{
	constexpr int DefaultCapacity = 128;

	Account* AccountList = new Account[DefaultCapacity];
	unsigned int CurrentLast = 0;

	while (true)
	{
		int Select;

		std::cout << "\n----- Menu -----\n" <<
			"1. 계좌개설\n" <<
			"2. 입금\n" <<
			"3. 출금\n" <<
			"4. 전체고객 잔액조회\n" <<
			"5. 프로그램 종료\n" << 
			"선택 : ";
		std::cin >> Select;

		unsigned int AccountNumber = 0;
		char Name[512];
		unsigned int Money = 0;

		switch (Select)
		{
		case 1: // 계좌개설
			std::cout << "\n[계좌개설]\n" <<
				"계좌번호 : " << CurrentLast << std::endl;
			std::cout << "이름 : ";
			std::cin >> Name;
			std::cout << "입금액 : ";
			std::cin >> Money;
			AccountList[CurrentLast].SetAccount(CurrentLast, Name, Money);
			CurrentLast++;
			break;
		case 2: // 입금
			std::cout << "\n[입금]\n" <<
				"계좌번호 : ";
			std::cin >> AccountNumber;
			if (AccountNumber >= CurrentLast)
			{
				std::cout << "없는 계좌번호 입니다." << std::endl;
				break;
			}
			std::cout << "이름 : " << AccountList[AccountNumber].GetName() << std::endl;
			std::cout << "입금액 : ";
			std::cin >> Money;
			AccountList[AccountNumber].SetCurrentMoney(AccountList[AccountNumber].GetCurrentMoney() + Money);
			std::cout << "고객님의 잔액은 " << AccountList[AccountNumber].GetCurrentMoney() << "원입니다." << std::endl;
			break;
		case 3: // 출금
			std::cout << "\n[출금]\n" <<
				"계좌번호 : ";
			std::cin >> AccountNumber;
			if (AccountNumber >= CurrentLast)
			{
				std::cout << "없는 계좌번호 입니다." << std::endl;
				break;
			}
			std::cout << "이름 : " << AccountList[AccountNumber].GetName() << std::endl;
			std::cout << "입금액 : ";
			std::cin >> Money;
			if (AccountList[AccountNumber].GetCurrentMoney() < Money)
			{
				std::cout << "잔액이 부족합니다." << std::endl;
				break;
			}
			AccountList[AccountNumber].SetCurrentMoney(AccountList[AccountNumber].GetCurrentMoney() - Money);
			std::cout << "고객님의 잔액은 " << AccountList[AccountNumber].GetCurrentMoney() << "원입니다." << std::endl;
			break;
		case 4: // 전체고객 잔액조회
			std::cout << "\n[전체고객 잔액조회]" << std::endl;
			for (unsigned int i = 0; i < CurrentLast; ++i)
			{
				std::cout << "계좌번호 : " << AccountList[i].GetAccountNumber() << std::endl;
				std::cout << "이름 : " << AccountList[i].GetName() << std::endl;
				std::cout << "잔액 : " << AccountList[i].GetCurrentMoney() << std::endl;
				std::cout << std::endl;
			}
			break;
		case 5: // 프로그램 종료
			return 0;
		default:
			break;
		}
	}

	delete[] AccountList;
	AccountList = nullptr;

	return 0;
}

