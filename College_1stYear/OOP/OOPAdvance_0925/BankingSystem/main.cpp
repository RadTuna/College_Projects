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
				"���¹�ȣ : " << CurrentLast << std::endl;
			std::cout << "�̸� : ";
			std::cin >> Name;
			std::cout << "�Աݾ� : ";
			std::cin >> Money;
			AccountList[CurrentLast].SetAccount(CurrentLast, Name, Money);
			CurrentLast++;
			break;
		case 2: // �Ա�
			std::cout << "\n[�Ա�]\n" <<
				"���¹�ȣ : ";
			std::cin >> AccountNumber;
			if (AccountNumber >= CurrentLast)
			{
				std::cout << "���� ���¹�ȣ �Դϴ�." << std::endl;
				break;
			}
			std::cout << "�̸� : " << AccountList[AccountNumber].GetName() << std::endl;
			std::cout << "�Աݾ� : ";
			std::cin >> Money;
			AccountList[AccountNumber].SetCurrentMoney(AccountList[AccountNumber].GetCurrentMoney() + Money);
			std::cout << "������ �ܾ��� " << AccountList[AccountNumber].GetCurrentMoney() << "���Դϴ�." << std::endl;
			break;
		case 3: // ���
			std::cout << "\n[���]\n" <<
				"���¹�ȣ : ";
			std::cin >> AccountNumber;
			if (AccountNumber >= CurrentLast)
			{
				std::cout << "���� ���¹�ȣ �Դϴ�." << std::endl;
				break;
			}
			std::cout << "�̸� : " << AccountList[AccountNumber].GetName() << std::endl;
			std::cout << "�Աݾ� : ";
			std::cin >> Money;
			if (AccountList[AccountNumber].GetCurrentMoney() < Money)
			{
				std::cout << "�ܾ��� �����մϴ�." << std::endl;
				break;
			}
			AccountList[AccountNumber].SetCurrentMoney(AccountList[AccountNumber].GetCurrentMoney() - Money);
			std::cout << "������ �ܾ��� " << AccountList[AccountNumber].GetCurrentMoney() << "���Դϴ�." << std::endl;
			break;
		case 4: // ��ü�� �ܾ���ȸ
			std::cout << "\n[��ü�� �ܾ���ȸ]" << std::endl;
			for (unsigned int i = 0; i < CurrentLast; ++i)
			{
				std::cout << "���¹�ȣ : " << AccountList[i].GetAccountNumber() << std::endl;
				std::cout << "�̸� : " << AccountList[i].GetName() << std::endl;
				std::cout << "�ܾ� : " << AccountList[i].GetCurrentMoney() << std::endl;
				std::cout << std::endl;
			}
			break;
		case 5: // ���α׷� ����
			return 0;
		default:
			break;
		}
	}

	delete[] AccountList;
	AccountList = nullptr;

	return 0;
}

