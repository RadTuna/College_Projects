#include <iostream>
#include "Account.h"
#include "AccountHandler.h"

int main()
{
	AccountHandler AccountData;

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
		{
			int AccountChoice = 0;
			std::cout << "\n[계좌개설]\n" <<
				"1. 보통예금계좌  2. 신용계좌 : ";
			std::cin >> AccountChoice;
			if (AccountChoice == 1)
			{
				std::cout << "[보통예금계좌]" << std::endl;
			}
			else if (AccountChoice == 2)
			{
				std::cout << "[신용예금계좌]" << std::endl;
			}
			else
			{
				std::cout << "없는 계좌 유형입니다." << std::endl;
				break;
			}

			std::cout << "계좌번호 : ";
			std::cin >> AccountNumber;
			if (AccountData.IsValidAccount(AccountNumber) == false || AccountData.IsEmptyAccount(AccountNumber) == false)
			{
				std::cout << "사용할 수 없는 계좌번호 입니다." << std::endl;
				break;
			}
			std::cout << "이름 : ";
			std::cin >> Name;
			std::cout << "입금액 : ";
			std::cin >> Money;
			if (AccountChoice == 1)
			{
				AccountData.NewNormalAccount(AccountNumber, Name, Money);
			}
			else if (AccountChoice == 2)
			{
				char CreditRating;
				std::cout << "신용등급(A, B, C) : ";
				std::cin >> CreditRating;
				AccountData.NewHighCreditAccount(AccountNumber, Name, Money, CreditRating);
			}
			break;
		}
		case 2: // 입금
		{
			std::cout << "\n[입금]\n" <<
				"계좌번호 : ";
			std::cin >> AccountNumber;
			if (AccountData.IsValidAccount(AccountNumber) == false || AccountData.IsEmptyAccount(AccountNumber) == true)
			{
				std::cout << "없는 계좌번호 입니다." << std::endl;
				break;
			}
			std::cout << "이름 : " << AccountData.GetAccountName(AccountNumber) << std::endl;
			std::cout << "입금액 : ";
			std::cin >> Money;
			AccountData.DespoitInAccount(AccountNumber, Money);
			std::cout << "고객님의 잔액은 " << AccountData.GetAccountMoney(AccountNumber) << "원입니다." << std::endl;
			break;
		}
		case 3: // 출금
		{
			std::cout << "\n[출금]\n" <<
				"계좌번호 : ";
			std::cin >> AccountNumber;
			if (AccountData.IsValidAccount(AccountNumber) == false || AccountData.IsEmptyAccount(AccountNumber) == true)
			{
				std::cout << "없는 계좌번호 입니다." << std::endl;
				break;
			}
			std::cout << "이름 : " << AccountData.GetAccountName(AccountNumber) << std::endl;
			std::cout << "출금액 : ";
			std::cin >> Money;
			if (AccountData.WithdrawInAccount(AccountNumber, Money) == false)
			{
				std::cout << "잔액이 부족합니다." << std::endl;
				break;
			}
			std::cout << "고객님의 잔액은 " << AccountData.GetAccountMoney(AccountNumber) << "원입니다." << std::endl;
			break;
		}
		case 4: // 전체고객 잔액조회
		{
			std::cout << "\n[전체고객 잔액조회]" << std::endl;
			for (unsigned int AccNum = 0; AccNum < (unsigned int)AccountData.GetDataSize(); ++AccNum)
			{
				char* AccName;
				unsigned int AccMoney;

				if (AccountData.GetAccountData(AccNum, AccName, AccMoney) == false)
				{
					continue;
				}

				std::cout << "계좌번호 : " << AccNum << std::endl;
				std::cout << "이름 : " << AccName << std::endl;
				std::cout << "잔액 : " << AccMoney << std::endl;
				std::cout << std::endl;
			}
			break;
		}
		case 5: // 프로그램 종료
			return 0;
		}
	}

	return 0;
}

