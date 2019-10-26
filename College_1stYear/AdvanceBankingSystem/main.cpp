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
			"1. ���°���\n" <<
			"2. �Ա�\n" <<
			"3. ���\n" <<
			"4. ��ü�� �ܾ���ȸ\n" <<
			"5. ���α׷� ����\n" << 
			"���� : ";
		std::cin >> Select;

		unsigned int AccountNumber = 0;
		char Name[512];
		unsigned int Money = 0;

		switch (Select)
		{
		case 1: // ���°���
			std::cout << "\n[���°���]\n" <<
				"���¹�ȣ : ";
			std::cin >> AccountNumber;
			if (AccountData.IsValidAccount(AccountNumber) == false || AccountData.IsEmptyAccount(AccountNumber) == false)
			{
				std::cout << "����� �� ���� ���¹�ȣ �Դϴ�." << std::endl;
				break;
			}
			std::cout << "�̸� : ";
			std::cin >> Name;
			std::cout << "�Աݾ� : ";
			std::cin >> Money;
			AccountData.NewAccount(AccountNumber, Name, Money);
			break;
		case 2: // �Ա�
			std::cout << "\n[�Ա�]\n" <<
				"���¹�ȣ : ";
			std::cin >> AccountNumber;
			if (AccountData.IsValidAccount(AccountNumber) == false || AccountData.IsEmptyAccount(AccountNumber) == true)
			{
				std::cout << "���� ���¹�ȣ �Դϴ�." << std::endl;
				break;
			}
			std::cout << "�̸� : " << AccountData.GetAccountName(AccountNumber) << std::endl;
			std::cout << "�Աݾ� : ";
			std::cin >> Money;
			AccountData.DespoitInAccount(AccountNumber, Money);
			std::cout << "������ �ܾ��� " << AccountData.GetAccountMoney(AccountNumber) << "���Դϴ�." << std::endl;
			break;
		case 3: // ���
			std::cout << "\n[���]\n" <<
				"���¹�ȣ : ";
			std::cin >> AccountNumber;
			if (AccountData.IsValidAccount(AccountNumber) == false || AccountData.IsEmptyAccount(AccountNumber) == true)
			{
				std::cout << "���� ���¹�ȣ �Դϴ�." << std::endl;
				break;
			}
			std::cout << "�̸� : " << AccountData.GetAccountName(AccountNumber) << std::endl;
			std::cout << "��ݾ� : ";
			std::cin >> Money;
			if (AccountData.WithdrawInAccount(AccountNumber, Money) == false)
			{
				std::cout << "�ܾ��� �����մϴ�." << std::endl;
				break;
			}
			std::cout << "������ �ܾ��� " << AccountData.GetAccountMoney(AccountNumber) << "���Դϴ�." << std::endl;
			break;
		case 4: // ��ü�� �ܾ���ȸ
			std::cout << "\n[��ü�� �ܾ���ȸ]" << std::endl;
			for (unsigned int AccNum = 0; AccNum < (unsigned int)AccountData.GetDataSize(); ++AccNum)
			{
				char* AccName;
				unsigned int AccMoney;

				if (AccountData.GetAccountData(AccNum, AccName, AccMoney) == false)
				{
					continue;
				}

				std::cout << "���¹�ȣ : " << AccNum << std::endl;
				std::cout << "�̸� : " << AccName << std::endl;
				std::cout << "�ܾ� : " << AccMoney << std::endl;
				std::cout << std::endl;
			}
			break;
		case 5: // ���α׷� ����
			return 0;
		}
	}

	return 0;
}

